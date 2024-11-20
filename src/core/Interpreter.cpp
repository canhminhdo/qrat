//
// Created by CanhDo on 2024/11/15.
//

#include "core/Interpreter.hpp"

void Interpreter::setCurrentProg(Token progName) {
    currentProg = new SyntaxProg(progName);
}

SyntaxProg *Interpreter::getCurrentProg() const {
    return currentProg;
}
