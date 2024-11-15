//
// Created by CanhDo on 2024/11/15.
//

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "core/SyntaxProg.hpp"
#include "core/Token.hpp"

class Interpreter {
public:
    void setCurrentProg(Token progName);
    SyntaxProg *getCurrentProg() const;

private:
    SyntaxProg *currentProg;
};
#endif//INTERPRETER_HPP
