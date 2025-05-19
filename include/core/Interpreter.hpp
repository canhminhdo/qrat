//
// Created by CanhDo on 2024/11/15.
//

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "model/Runner.hpp"
#include "StateTransitionGraph.hpp"
#include "StateTransitionGraph2.hpp"
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

    void initGraphSearch2(char* property);

    void execute();

    void execute2();

    void initializeSearch(int progName, ExpNode *propExp, Search::Type type, int numSols, int maxDepth);

    void initializeSearch2(int progName, char* property, std::vector<char *> *args);

    void initRunner(std::vector<char *> *args);

    void cleanSearch();

    void cleanSearch2();

    void finalizeProg();

    void showPath(int stateId);

private:
    SyntaxProg *currentProg;
    DDSimulation *ddSim;
    StateTransitionGraph *graphSearch;
    StateTransitionGraph2 *graphSearch2;
    Runner *runner;
    std::unordered_map<int, SyntaxProg *> savedProgs;
};
#endif//INTERPRETER_HPP
