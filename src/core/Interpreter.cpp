//
// Created by CanhDo on 2024/11/15.
//

#include "core/Interpreter.hpp"
#include <iostream>

void Interpreter::setCurrentProg(Token progName) {
    currentProg = new SyntaxProg(progName);
}

SyntaxProg *Interpreter::getCurrentProg() const {
    return currentProg;
}

void Interpreter::initDDSimulation() {
    if (currentProg) {
        ddSim = new DDSimulation(currentProg);
    } else {
        std::cerr << "Error: no program to simulate." << std::endl;
    }
}

void Interpreter::initGraphSearch(ExpNode *propExp, Search::Type type, int numSols, int maxDepth) {
    graphSearch = new StateTransitionGraph(currentProg, ddSim, propExp, type, numSols, maxDepth);
}

void Interpreter::execute() {
    assert(ddSim != nullptr);
    assert(graphSearch != nullptr);
    // ddSim->dump();
    // graphSearch->dump();
    graphSearch->search();
}

void Interpreter::buildInitialState() {
}

void Interpreter::initializeSearch(int progName, ExpNode *propExp, Search::Type type, int numSols, int maxDepth) {
    if (progName != currentProg->getName()) {
        throw std::runtime_error("Error: program name does not match.");
    }
    initDDSimulation();
    initGraphSearch(propExp, type, numSols, maxDepth);
}