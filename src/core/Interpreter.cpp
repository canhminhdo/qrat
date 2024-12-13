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

void Interpreter::run() {
    if (currentProg) {
        // currentProg->dump();
        initDDSimulation();
    }
    if (ddSim)
        ddSim->dump();
}
