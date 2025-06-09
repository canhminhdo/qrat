//
// Created by CanhDo on 2024/11/15.
//

#include "core/Interpreter.hpp"
#include "model/DTMC.hpp"
#include "model/PrismRunner.hpp"
#include "utility/Tty.hpp"
#include <core/StateSpaceGraph.hpp>
#include <iostream>
#include "model/RunnerFactory.hpp"

void Interpreter::setCurrentProg(Token progName) {
    currentProg = new SyntaxProg(progName);
}

SyntaxProg *Interpreter::getCurrentProg() const {
    return currentProg;
}

bool Interpreter::existProg(Token progName) {
    auto savedProg = savedProgs.find(progName.code());
    if (savedProg == savedProgs.end()) {
        return false;
    }
    currentProg = savedProg->second;
    return true;
}

void Interpreter::initDDSimulation() {
    if (currentProg) {
        ddSim = new DDSimulation(currentProg);
    } else {
        std::cerr << "Error: No program to simulate" << std::endl;
    }
}

void Interpreter::initGraphSearch(ExpNode *propExp, Search::Type type, int numSols, int maxDepth, bool probMode) {
    graphSearch = new StateTransitionGraph(currentProg, ddSim, propExp, type, numSols, maxDepth, probMode);
}

void Interpreter::initGraphSearch(char *property, std::vector<char *> *args) {
    graphSearch = new StateSpaceGraph(currentProg, ddSim, property, args);
}

void Interpreter::execute() {
    assert(ddSim != nullptr);
    assert(graphSearch != nullptr);
    graphSearch->printCommand();
    graphSearch->search();
}

void Interpreter::executePCheck() {
    assert(ddSim != nullptr);
    assert(graphSearch != nullptr);
    assert(runner != nullptr);
    Timer timer(true);
    auto *stateSpaceGraph = dynamic_cast<StateSpaceGraph *>(graphSearch);
    if (runner->isAvailable() && stateSpaceGraph != nullptr) {
        stateSpaceGraph->printCommand();
        stateSpaceGraph->search();
        auto dtmc = DTMC(currentProg, stateSpaceGraph);
        dtmc.buildModel();
        runner->modelCheck(dtmc.getFileModel()->getFileName(), std::string(stateSpaceGraph->getProperty()));
        if (!runner->getSaveModel()) {
            dtmc.cleanup();
        }
        timer.total();
    }
    delete runner;
    timer.stop();
}

void Interpreter::initializeSearch(int progName, ExpNode *propExp, Search::Type type, int numSols, int maxDepth, bool probMode) {
    assert(currentProg != nullptr && progName == currentProg->getName());
    cleanSearch();
    initDDSimulation();
    initGraphSearch(propExp, type, numSols, maxDepth, probMode);
}

void Interpreter::cleanSearch() {
    if (graphSearch != nullptr) {
        delete graphSearch;
        graphSearch = nullptr;
    }
    if (ddSim != nullptr) {
        delete ddSim;
        ddSim = nullptr;
    }
}

void Interpreter::initializePCheck(int progName, char *property, std::vector<char *> *args) {
    assert(currentProg != nullptr && progName == currentProg->getName());
    cleanSearch();
    initDDSimulation();
    initGraphSearch(property, args);
    initRunner(args);
}

void Interpreter::initRunner(std::vector<char *> *args) {
    runner = RunnerFactory::createRunner(args);
}

void Interpreter::finalizeProg() {
    assert(currentProg != nullptr);
    auto oldProg = savedProgs.find(currentProg->getName());
    if (oldProg == savedProgs.end()) {
        savedProgs.insert({currentProg->getName(), currentProg});
        std::cout << "==========================================\n";
        std::cout << "prog " << Token::name(currentProg->getName()) << std::endl;
    } else {
        delete oldProg->second;
        oldProg->second = currentProg;
        std::cout << "==========================================\n";
        std::cout << "prog " << Token::name(currentProg->getName()) << '\n';
        std::cout << Tty(Tty::GREEN) << "Advisory: " << Tty(Tty::RESET) << "redefining program " << Tty(Tty::MAGENTA) << Token::name(currentProg->getName()) << Tty(Tty::RESET) << "." << std::endl;
    }
}
void Interpreter::showPath(int stateId) {
    if (graphSearch != nullptr) {
        graphSearch->showPath(stateId);
    } else {
        std::cout << Tty(Tty::RED) << "Warning: " << Tty(Tty::RESET) << "no state graph." << std::endl;
    }
}