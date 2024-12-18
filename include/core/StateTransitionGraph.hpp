//
// Created by CanhDo on 2024/12/17.
//

#ifndef STATETRANSITIONGRAPH_HPP
#define STATETRANSITIONGRAPH_HPP

#include <unordered_set>
#include "utility/HashUtil.hpp"
#include "ast/StmNode.hpp"
#include "dd/DDSimulation.hpp"

class StateTransitionGraph {
public:
    StateTransitionGraph(SyntaxProg *currentProg, DDSimulation *ddSim);

    struct State {
        int stateNr;
        StmNode *pc; // program counter
        qc::VectorDD current{}; // current quantum state
        int parent;
        std::vector<int> nextStates;
        bool fullyExplored;

        State(StmNode *pc, qc::VectorDD current, int parent = -1)
            : pc{pc}, current{std::move(current)}, parent{parent}, fullyExplored{false} {
        }
        void setId(int id) {
            stateNr = id;
        }
    };

    struct StateHash {
        std::size_t operator()(const State *s) const {
            return HashUtil::combinedHash(std::hash<StmNode *>()(s->pc),
                                          std::hash<dd::vNode *>()(dynamic_cast<dd::vNode *>(s->current.p)));
        }
    };

    struct StateEqual {
        bool operator()(const State *lhs, const State *rhs) const {
            return lhs->pc == rhs->pc && lhs->current.p == rhs->current.p;
        }
    };

    void buildInitialState();

    void execute();

    State *makeState(State *s, bool &found);

    bool exist(State *s);

    State *getNextState();

    void dump() const;

    void printState(State *s) const;

private:
    // for analysis
    std::unordered_map<State *, int, StateHash, StateEqual> stateTab;
    std::vector<State *> seenStates;
    int savedStateId{-1}; // state being considered

    SyntaxProg *currentProg;
    DDSimulation *ddSim;
};

#endif //STATETRANSITIONGRAPH_HPP
