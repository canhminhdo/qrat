//
// Created by CanhDo on 2024/11/15.
//

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "StateTransitionGraph.hpp"
#include "core/SyntaxProg.hpp"
#include "core/Token.hpp"
#include "dd/DDSimulation.hpp"


class Interpreter {
public:
    Interpreter() = default;

    void setCurrentProg(Token progName);

    SyntaxProg *getCurrentProg() const;

    void initDDSimulation();

    void initGraphSearch(ExpNode *propExp, Search::Type type, int numSols, int maxDepth);

    void execute();

    void buildInitialState();

    void initializeSearch(int progName, ExpNode *propExp, Search::Type type, int numSols, int maxDepth);

private:
    SyntaxProg *currentProg;
    DDSimulation *ddSim;
    StateTransitionGraph *graphSearch;
};
#endif//INTERPRETER_HPP
