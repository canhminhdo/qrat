//
// Created by CanhDo on 2024/12/18.
//

#include "core/StateTransitionGraph.hpp"

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
    int solNr = 1;
    int found = 0;

    assert(savedStateId < seenStates.size());
    State *s = seenStates[savedStateId];
    if (s->pc == nullptr) {
        // there are no more statements to execute
        std::cout << "No more statements to execute\n";
    }
    if (dynamic_cast<UnitaryStmNode *>(s->pc)) {
        std::cout << "Unitary transformation\n";
        s->pc->dump(false);
        auto v = ddSim->applyGate(s->pc, s->current);
        v.printVector<dd::vNode>();
        State *newState = new State(s->pc->getNext(), v, s->stateNr);
        bool inCached = false;
        State *s1 = makeState(newState, inCached);
        if (inCached) {
            std::cout << "State already exists\n";
        } else {
            s->nextStates.push_back(s1->stateNr);
            std::cout << "New state created: " << s1->stateNr << "\n";
        }
    }

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
