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

class StateTransitionGraph {
public:
    StateTransitionGraph(SyntaxProg *currentProg, DDSimulation *ddSim);

    enum class SearchType {
        ONE_STEP,
        ZERO_OR_MORE_STEPS,
        ONE_OR_MORE_STEPS,
        FINAL_STATES
    };

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

    void execute();

    void processState(State *currentState, std::vector<State *> &results);

    void processSkipStm([[maybe_unused]] SkipStmNode *skipStm, State *currentState, StmNode *nextStm,
                        std::vector<State *> &results);

    void processUnitaryStm(UnitaryStmNode *unitaryStm, State *currentState, StmNode *nextStm,
                           std::vector<State *> &results);

    void processCondStm(CondStmNode *condStm, State *currentState, std::vector<State *> &results);

    void processWhileStm(WhileStmNode *whileStm, State *currentState, StmNode *nextStm, std::vector<State *> &results);

    void processCondBranch(State *currentState, StmNode *nextStm, qc::VectorDD &v, std::vector<State *> &results);

    StmNode *getNextStatement(StmNode *stm);

    void showPath(int stateId);

    std::pair<State *, bool> makeState(State *s);

    void checkState(State *s, std::vector<State *> &results);

    bool exist(State *s);

    State *getNextState();

    void dump() const;

    void printState(State *s, bool recursive = true) const;

private:
    // for analysis
    std::unordered_map<State *, int, StateHash, StateEqual> stateTab;
    std::vector<State *> seenStates;
    int savedStateId{-1}; // state being considered

    SyntaxProg *currentProg;
    DDSimulation *ddSim;

    SearchType searchType{SearchType::FINAL_STATES};
    int numSols{UNBOUNDED};
    int depthBound{UNBOUNDED};
};

#endif //STATETRANSITIONGRAPH_HPP
