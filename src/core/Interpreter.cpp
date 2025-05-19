//
// Created by CanhDo on 2024/11/15.
//

#include "core/Interpreter.hpp"
#include "model/DTMC.hpp"
#include "utility/Tty.hpp"
#include <iostream>
#include <model/StormRunner.hpp>
#include "model/PrismRunner.hpp"

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

void Interpreter::initGraphSearch(ExpNode *propExp, Search::Type type, int numSols, int maxDepth) {
    graphSearch = new StateTransitionGraph(currentProg, ddSim, propExp, type, numSols, maxDepth);
}

void Interpreter::initGraphSearch2(char *property) {
    graphSearch2 = new StateTransitionGraph2(currentProg, ddSim, property);
}

void Interpreter::execute() {
    assert(ddSim != nullptr);
    assert(graphSearch != nullptr);
    // ddSim->dump();
    // graphSearch->dump();
    graphSearch->printSearchCommand();
    graphSearch->search();
}

void Interpreter::execute2() {
    assert(ddSim != nullptr);
    assert(graphSearch2 != nullptr);
    assert(runner != nullptr);
    // ddSim->dump();
    // graphSearch->dump();
    Timer timer(true);
    if (runner->isAvailable()) {
        graphSearch2->printSearchCommand();
        graphSearch2->search();
        auto dtmc = DTMC(currentProg, graphSearch2);
        dtmc.buildModel();
        runner->modelCheck(dtmc.getFileModel()->getFileName(), std::string(graphSearch2->getProperty()));
        timer.total();
    }
    delete runner;
    timer.stop();
}

void Interpreter::initializeSearch(int progName, ExpNode *propExp, Search::Type type, int numSols, int maxDepth) {
    assert(currentProg != nullptr && progName == currentProg->getName());
    cleanSearch();
    initDDSimulation();
    initGraphSearch(propExp, type, numSols, maxDepth);
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

void Interpreter::cleanSearch2() {
    if (graphSearch2 != nullptr) {
        delete graphSearch2;
        graphSearch2 = nullptr;
    }
    if (ddSim != nullptr) {
        delete ddSim;
        ddSim = nullptr;
    }
}

void Interpreter::initializeSearch2(int progName, char *property, std::vector<char *> *args) {
    assert(currentProg != nullptr && progName == currentProg->getName());
    cleanSearch2();
    initDDSimulation();
    initGraphSearch2(property);
    initRunner(args);
}

void Interpreter::initRunner(std::vector<char *> *args) {
    enum Flag {
        PRISM,
        STORM
    };
    Flag flag = Flag::PRISM;
    for (int i = 0; i < args->size(); i++) {
        if (strcmp(args->at(i), "--backend=Storm") == 0) {
            flag = Flag::STORM;
        } else if (strcmp(args->at(i), "--backend=PRISM") == 0) {
            flag = Flag::PRISM;
        }
        delete args->at(i);
    }
    if (flag == Flag::PRISM) {
        runner = new PrismRunner();
    } else if (flag == Flag::STORM) {
        runner = new StormRunner();
    }
}

void Interpreter::finalizeProg() {
    assert(currentProg != nullptr);
    auto oldProg = savedProgs.find(currentProg->getName());
    if (oldProg == savedProgs.end()) {
        savedProgs.insert({currentProg->getName(), currentProg});
        std::cout << "==========================================" << std::endl;
        std::cout << "prog " << Token::name(currentProg->getName()) << std::endl;
    } else {
        delete oldProg->second;
        oldProg->second = currentProg;
        std::cout << "==========================================" << std::endl;
        std::cout << "prog " << Token::name(currentProg->getName()) << std::endl;
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