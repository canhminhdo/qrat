//
// Created by CanhDo on 2024/12/17.
//

#ifndef STATETRANSITIONGRAPH_HPP
#define STATETRANSITIONGRAPH_HPP

#include "ast/CondStmNode.hpp"
#include "ast/EndStmNode.hpp"
#include "ast/StmNode.hpp"
#include "ast/SkipStmNode.hpp"
#include "ast/WhileStmNode.hpp"
#include "dd/DDSimulation.hpp"
#include "utility/HashUtil.hpp"
#include "utility/macros.hpp"
#include "Search.hpp"

class StateTransitionGraph : public Search {
public:
    StateTransitionGraph(SyntaxProg *currentProg, DDSimulation *ddSim);

    struct State {
        int stateNr;
        StmNode *pc; // program counter
        qc::VectorDD current{}; // current quantum state
        int parent;
        std::vector<int> nextStates;
        int depth{0};

        State(StmNode *pc, qc::VectorDD current, int parent = -1, int depth = 0)
            : pc{pc}, current{std::move(current)}, parent{parent}, depth{depth} {
        }

        bool isFinalState() {
            auto *endStm = dynamic_cast<EndStmNode *>(pc);
            if (endStm != nullptr && endStm->getNext() == nullptr) {
                return true;
            }
            return false;
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

    void search();

    void procState(State *currentState, std::unordered_set<int> &results);

    void procSkipStm([[maybe_unused]] SkipStmNode *skipStm, State *currentState, StmNode *nextStm,
                     std::unordered_set<int> &results);

    void procUnitaryStm(UnitaryStmNode *unitaryStm, State *currentState, StmNode *nextStm,
                        std::unordered_set<int> &results);

    void procCondStm(CondStmNode *condStm, State *currentState, std::unordered_set<int> &results);

    void procWhileStm(WhileStmNode *whileStm, State *currentState, StmNode *nextStm, std::unordered_set<int> &results);

    void procCondBranch(State *currentState, StmNode *nextStm, qc::VectorDD &v, std::unordered_set<int> &results);

    StmNode *getNextStatement(StmNode *stm);

    void showPath(int stateId);

    std::pair<State *, bool> makeState(State *s);

    void checkState(State *s, std::unordered_set<int> &results);

    void dump() const;

    void printState(State *s, bool recursive = true) const;

private:
    // for analysis
    std::unordered_map<State *, int, StateHash, StateEqual> stateTab;
    std::vector<State *> seenStates;
    int savedStateId{-1}; // state being considered

    SyntaxProg *currentProg;
    DDSimulation *ddSim;

    Type searchType{Type::ARROW_EXCLAMATION};
    int numSols{UNBOUNDED};
    int depthBound{UNBOUNDED};
};

#endif //STATETRANSITIONGRAPH_HPP
