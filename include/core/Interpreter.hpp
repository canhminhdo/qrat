//
// Created by CanhDo on 2024/11/15.
//

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "core/SyntaxProg.hpp"
#include "core/Token.hpp"
#include "dd/DDSimulation.hpp"

class Interpreter {
public:
    Interpreter() = default;

    void setCurrentProg(Token progName);

    SyntaxProg *getCurrentProg() const;

    void initDDSimulation();

    void run();

private:
    SyntaxProg *currentProg;
    DDSimulation *ddSim;
};
#endif//INTERPRETER_HPP
