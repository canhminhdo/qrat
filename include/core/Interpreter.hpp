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

    bool existProg(Token progName);

    void initDDSimulation();

    void initGraphSearch(ExpNode *propExp, Search::Type type, int numSols, int maxDepth);

    void execute();

    void initializeSearch(int progName, ExpNode *propExp, Search::Type type, int numSols, int maxDepth);

    void finalizeProg();

private:
    SyntaxProg *currentProg;
    DDSimulation *ddSim;
    StateTransitionGraph *graphSearch;
    std::unordered_map<int, SyntaxProg *> savedProgs;
};
#endif//INTERPRETER_HPP
