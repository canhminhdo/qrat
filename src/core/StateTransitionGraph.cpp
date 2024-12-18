//
// Created by CanhDo on 2024/12/18.
//

#include "core/StateTransitionGraph.hpp"
#include "ast/CondStmNode.hpp"
#include "ast/EndStmNode.hpp"
#include "ast/SkipStmNode.hpp"
#include "ast/WhileStmNode.hpp"

StateTransitionGraph::StateTransitionGraph(SyntaxProg *currentProg, DDSimulation *ddSim) {
    this->currentProg = currentProg;
    this->ddSim = ddSim;
}

void StateTransitionGraph::buildInitialState() {
    auto *initialState = new State(currentProg->getStmSeq()->getHead(), ddSim->getInitialState());
    assert(seenStates.empty());
    bool inCached = false;
    State *s = makeState(initialState, inCached);
    s->setId(0);
    savedStateId = 0;
}

void StateTransitionGraph::execute() {
    buildInitialState();
    std::vector<State *> results;
    int numSols = 1;
    int found = 0;

    assert(savedStateId < seenStates.size());
    while (savedStateId < seenStates.size()) {
        State *currentState = seenStates[savedStateId];
        StmNode *stm = currentState->pc;
        if (dynamic_cast<EndStmNode *>(stm)) {
            std::cout << "End of program\n";
            savedStateId++;
            continue;
        }
        StmNode *nextStm = stm->getNext();
        auto *endStm = dynamic_cast<EndStmNode *>(stm->getNext());
        if (endStm != nullptr && endStm->getNext() != nullptr) {
            nextStm = endStm->getNext();
        }
        // SkipStmNode
        auto *skipStm = dynamic_cast<SkipStmNode *>(nextStm);
        if (skipStm != nullptr) {
            std::cout << "Skip statement\n";
            savedStateId++;
            continue;
        }
        // UnitaryStmNode
        auto *unitaryStm = dynamic_cast<UnitaryStmNode *>(nextStm);
        if (unitaryStm != nullptr) {
            std::cout << "Unitary transformation\n";
            savedStateId++;
            continue;
        }
        // CondStmNode
        auto *condStm = dynamic_cast<CondStmNode *>(nextStm);
        if (condStm != nullptr) {
            std::cout << "Conditional statement\n";
            savedStateId++;
            continue;
        }
        // WhileStmNode
        auto *whileStm = dynamic_cast<WhileStmNode *>(nextStm);
        if (whileStm != nullptr) {
            std::cout << "While statement\n";
            savedStateId++;
            continue;
        }
        throw std::runtime_error("Unknown statement type");
    }

//    if (s->pc == nullptr) {
//        // there are no more statements to execute
//        std::cout << "No more statements to execute\n";
//    }
//    if (dynamic_cast<UnitaryStmNode *>(s->pc)) {
//        std::cout << "Unitary transformation\n";
//        s->pc->dump(false);
//        auto v = ddSim->applyGate(s->pc, s->current);
//        v.printVector<dd::vNode>();
//        State *newState = new State(s->pc->getNext(), v, s->stateNr);
//        bool inCached = false;
//        State *s1 = makeState(newState, inCached);
//        if (inCached) {
//            std::cout << "State already exists\n";
//        } else {
//            s->nextStates.push_back(s1->stateNr);
//            std::cout << "New state created: " << s1->stateNr << "\n";
//        }
//    }

    // while (found < solNr) {
    //     State *s = seenStates[savedStateId];
    // if (ddSim->test(s->current)) {
    //     results.push_back(s);
    //     found++;
    // }
    //     // given DD state and the current program counter, we can determine the next state
    // }
}

StateTransitionGraph::State *StateTransitionGraph::makeState(State *s, bool &found) {
    auto it = stateTab.find(s);
    if (it != stateTab.end()) {
        found = true;
        std::cout << "Found state in cache, then delete the newly created state\n";
        std::cout << "-------------------\n";
        delete s;
        return seenStates[it->second];
    }
    found = false;
    s->stateNr = seenStates.size();
    stateTab[s] = s->stateNr;
    seenStates.push_back(s);
    return s;
}

bool StateTransitionGraph::exist(State *s) {
    auto it = stateTab.find(s);
    return it != stateTab.end();
}

StateTransitionGraph::State * StateTransitionGraph::getNextState() {
    return nullptr;
}

void StateTransitionGraph::dump() const {
    std::cout << "State Transition Graph\n";
    std::cout << "State ID being considered: " << savedStateId << "\n";
    std::cout << "-------------------\n";
    if (seenStates.size() != 0) {
        printState(seenStates[0]);
    }
}

void StateTransitionGraph::printState(State *s) const {
    if (s == nullptr) {
        return;
    }
    std::cout << "\n-------------------\n";
    std::cout << "[State ID]: " << s->stateNr << "\n";
    std::cout << "[Parent State ID]: " << s->parent << "\n";
    std::cout << "[Program Counter]: ";
    s->pc->dump(false);
    std::cout << "[Fully Explored]: " << s->fullyExplored << "\n";
    std::cout << "[Current Quantum State]: ";
    s->current.printVector<dd::vNode>();
    std::cout << "[Next States]: ";
    for (int i : s->nextStates) {
        printState(seenStates[i]);
    }
}
