//
// Created by CanhDo on 2024/12/17.
//

#ifndef STATETRANSITIONGRAPH_HPP
#define STATETRANSITIONGRAPH_HPP

#include "Search.hpp"
#include "ast/CondStmNode.hpp"
#include "ast/EndStmNode.hpp"
#include "ast/SkipStmNode.hpp"
#include "ast/StmNode.hpp"
#include "ast/WhileStmNode.hpp"
#include "dd/DDSimulation.hpp"
#include "utility/HashUtil.hpp"
#include "utility/Timer.hpp"
#include "utility/macros.hpp"

class StateTransitionGraph : public Search {
public:
    StateTransitionGraph(SyntaxProg *currentProg, DDSimulation *ddSim, ExpNode *propExp, Search::Type type, int numSols,
                         int maxDepth);

    struct State {
        int stateNr;
        StmNode *pc;           // program counter
        qc::VectorDD current{};// current quantum state
        int parent;
        std::vector<int> nextStates;
        int depth{0};
        qc::fp prob{1.0};
        int outcome{0};

        State(StmNode *pc, qc::VectorDD current, int parent = -1, int depth = 0, qc::fp prob = 1.0)
            : pc{pc}, current{std::move(current)}, parent{parent}, depth{depth}, prob{prob} {
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

        virtual bool hasOutcome() const {
            return false;
        }

        virtual int getOutcome() const {
            throw std::runtime_error("No outcome available");
        }
    };

    struct StateWithOutcome : public State {
        int outcome{0};

        StateWithOutcome(StmNode *pc, qc::VectorDD current, int parent = -1, int depth = 0, qc::fp prob = 1.0, int outcome = 0)
            : State(pc, std::move(current), parent, depth, prob), outcome{outcome} {
        }

        bool hasOutcome() const override {
            return true;
        }
        int getOutcome() const override {
            return outcome;
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

    void procState(State *currentState, const Timer &timer);

    void procSkipStm([[maybe_unused]] SkipStmNode *skipStm, State *currentState, StmNode *nextStm, const Timer &timer);

    void procUnitaryStm(UnitaryStmNode *unitaryStm, State *currentState, StmNode *nextStm, const Timer &timer);

    void procCondStm(CondStmNode *condStm, State *currentState, const Timer &timer);

    void procWhileStm(WhileStmNode *whileStm, State *currentState, StmNode *nextStm, const Timer &timer);

    void procCondBranch(State *currentState, StmNode *nextStm, qc::VectorDD &v, qc::fp prob, int outcome, const Timer &timer);

    StmNode *getNextStatement(StmNode *stm);

    void showPath(int stateId, bool endState = true) const;

    std::pair<State *, bool> makeState(State *s);

    void checkState(State *s, const Timer &timer);

    const char *getSearchType() const;

    void printProbability(State *s) const;

    void printSearchTiming(State *s, const Timer &timer) const;

    void printSearchTiming(const Timer &timer) const;

    void printSearchCommand();

    void dump() const;

    void printState(State *s, bool recursive = true) const;

private:
    // for analysis
    std::unordered_map<State *, int, StateHash, StateEqual> stateTab;
    std::vector<State *> seenStates;
    int savedStateId{-1};// state being considered

    SyntaxProg *currentProg;
    DDSimulation *ddSim;

    ExpNode *propExp{nullptr};
    Type searchType{Type::ARROW_EXCLAMATION};
    int numSols{UNBOUNDED};
    int solutionCount{0};
    int depthBound{UNBOUNDED};
};

#endif//STATETRANSITIONGRAPH_HPP
