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
    makeState(initialState);
    savedStateId = 0;
}

void StateTransitionGraph::execute() {
    buildInitialState();
    int depth = 0;
    std::vector<State *> results;
    // numSols = 1;
    // depthBound = 1;
    // searchType = SearchType::ONE_OR_MORE_STEPS;
    while (results.size() < numSols && depth < depthBound && savedStateId < seenStates.size()) {
        State *currentState = seenStates[savedStateId];
        depth = currentState->depth;
        processState(currentState, results);
        savedStateId++;
    }
    std::cout << "Number of solutions found: " << results.size() << "\n";
    for (auto *s: results) {
        printState(s, false);
        // showPath(s->stateNr);
    }
    // buildInitialState();
    // std::vector<State *> results;
    // numSols = 1;
    // assert(savedStateId < seenStates.size());
    // while (results.size() <= numSols && savedStateId < seenStates.size()) {
    //     State *currentState = seenStates[savedStateId];
    //     if (currentState->depth >= depthBound) {
    //         break;
    //     }
    //     processState(currentState, results);
    //     int stateNr = currentState->stateNr;
    //     int depth = currentState->depth;
    //     StmNode *stm = currentState->pc;
    //     qc::VectorDD v = currentState->current;
    //     bool inCache = false;
    //     std::cout << "---------" << stateNr << "----------\n";
    //     // EndStmNode
    //     auto *endProg = dynamic_cast<EndStmNode *>(stm);
    //     if (endProg != nullptr && endProg->getNext() == nullptr) {
    //         std::cout << "End of program\n";
    //         savedStateId++;
    //         continue;
    //     }
    //
    //     // find next statement
    //     StmNode *nextStm = stm->getNext();
    //     auto *endStm = dynamic_cast<EndStmNode *>(stm->getNext());
    //     if (endStm != nullptr && endStm->getNext() != nullptr) {
    //         nextStm = endStm->getNext();
    //     }
    //     // SkipStmNode
    //     auto *skipStm = dynamic_cast<SkipStmNode *>(stm);
    //     if (skipStm != nullptr) {
    //         std::cout << "Skip statement\n";
    //         auto *newState = makeState(new State(nextStm, v, stateNr, depth + 1), inCache);
    //         if (!inCache) {
    //             currentState->nextStates.push_back(newState->stateNr);
    //             checkState(newState, results);
    //         }
    //         savedStateId++;
    //         continue;
    //     }
    //     // UnitaryStmNode
    //     auto *unitaryStm = dynamic_cast<UnitaryStmNode *>(stm);
    //     if (unitaryStm != nullptr) {
    //         std::cout << "Unitary transformation\n";
    //         auto v1 = ddSim->applyGate(unitaryStm, v);
    //         auto *newState = makeState(new State(nextStm, v1, stateNr, depth + 1), inCache);
    //         if (!inCache) {
    //             currentState->nextStates.push_back(newState->stateNr);
    //             checkState(newState, results);
    //         }
    //         savedStateId++;
    //         continue;
    //     }
    //     // CondStmNode
    //     auto *condStm = dynamic_cast<CondStmNode *>(stm);
    //     if (condStm != nullptr) {
    //         std::cout << "Conditional statement\n";
    //         // only support conditional measurement for now
    //         auto *measExp = dynamic_cast<MeasExpNode *>(condStm->getCond());
    //         assert(measExp != nullptr);
    //         auto [v0, v1] = ddSim->measure(measExp, v);
    //         if (!v0.isZeroTerminal()) {
    //             auto *newState = makeState(new State(condStm->getElseStm()->getHead(), v0, stateNr, depth + 1), inCache);
    //             if (!inCache) {
    //                 currentState->nextStates.push_back(newState->stateNr);
    //                 checkState(newState, results);
    //                 if (results.size() >= numSols) {
    //                     break;
    //                 }
    //             }
    //         }
    //         if (!v1.isZeroTerminal()) {
    //             inCache = false;
    //             auto *newState = makeState(new State(condStm->getThenStm()->getHead(), v1, stateNr, depth + 1), inCache);
    //             if (!inCache) {
    //                 currentState->nextStates.push_back(newState->stateNr);
    //                 checkState(newState, results);
    //             }
    //         }
    //         savedStateId++;
    //         continue;
    //     }
    //     // WhileStmNode
    //     auto *whileStm = dynamic_cast<WhileStmNode *>(stm);
    //     if (whileStm != nullptr) {
    //         std::cout << "While statement\n";
    //         // only support conditional measurement for now
    //         auto *measExp = dynamic_cast<MeasExpNode *>(whileStm->getCond());
    //         assert(measExp != nullptr);
    //         auto [v0, v1] = ddSim->measure(measExp, v);
    //         if (!v0.isZeroTerminal()) {
    //             auto *newState = makeState(new State(nextStm, v0, stateNr, depth + 1), inCache);
    //             if (!inCache) {
    //                 currentState->nextStates.push_back(newState->stateNr);
    //                 checkState(newState, results);
    //                 if (results.size() >= numSols) {
    //                     break;
    //                 }
    //             }
    //         }
    //         if (!v1.isZeroTerminal()) {
    //             inCache = false;
    //             auto *newState = makeState(new State(whileStm->getBody()->getHead(), v1, stateNr, depth + 1), inCache);
    //             if (!inCache) {
    //                 currentState->nextStates.push_back(newState->stateNr);
    //                 checkState(newState, results);
    //             }
    //         }
    //         savedStateId++;
    //         continue;
    //     }
    //     throw std::runtime_error("Unknown statement type");
    // }
    //
    // std::cout << "Number of solutions found: " << results.size() << "\n";
    // for (auto *s : results) {
    //     printState(s, false);
    // }
}

void StateTransitionGraph::processState(State *currentState, std::vector<State *> &results) {
    StmNode *stm = currentState->pc;
    if (auto *endProg = dynamic_cast<EndStmNode *>(stm); endProg != nullptr && endProg->getNext() == nullptr) {
        return;
    }
    StmNode *nextStm = getNextStatement(stm);
    if (auto *skipStm = dynamic_cast<SkipStmNode *>(stm); skipStm != nullptr) {
        processSkipStm(skipStm, currentState, nextStm, results);
    } else if (auto *unitaryStm = dynamic_cast<UnitaryStmNode *>(stm); unitaryStm != nullptr) {
        processUnitaryStm(unitaryStm, currentState, nextStm, results);
    } else if (auto *condStm = dynamic_cast<CondStmNode *>(stm); condStm != nullptr) {
        processCondStm(condStm, currentState, results);
    } else if (auto *whileStm = dynamic_cast<WhileStmNode *>(stm); whileStm != nullptr) {
        processWhileStm(whileStm, currentState, nextStm, results);
    } else {
        throw std::runtime_error("Unknown statement type");
    }
}

StmNode *StateTransitionGraph::getNextStatement(StmNode *stm) {
    StmNode *nextStm = stm->getNext();
    if (auto *endStm = dynamic_cast<EndStmNode *>(stm->getNext()); endStm != nullptr && endStm->getNext() != nullptr) {
        nextStm = endStm->getNext();
    }
    return nextStm;
}

void StateTransitionGraph::processSkipStm(SkipStmNode *skipStm, State *currentState, StmNode *nextStm, std::vector<State *> &results) {
    auto [newState, inCache] = makeState(new State(nextStm, currentState->current, currentState->stateNr,
                                                   currentState->depth + 1));
    if (!inCache) {
        currentState->nextStates.push_back(newState->stateNr);
        checkState(newState, results);
    }
}

void StateTransitionGraph::processUnitaryStm(UnitaryStmNode *unitaryStm, State *currentState, StmNode *nextStm,
                                             std::vector<State *> &results) {
    auto v = currentState->current;
    auto v1 = ddSim->applyGate(unitaryStm, v);
    auto [newState, inCache] = makeState(new State(nextStm, v1, currentState->stateNr, currentState->depth + 1));
    if (!inCache) {
        currentState->nextStates.push_back(newState->stateNr);
        checkState(newState, results);
    }
}

void StateTransitionGraph::processCondStm(CondStmNode *condStm, State *currentState, std::vector<State *> &results) {
    auto *measExp = dynamic_cast<MeasExpNode *>(condStm->getCond());
    assert(measExp != nullptr);
    auto [v0, v1] = ddSim->measure(measExp, currentState->current);
    processCondBranch(currentState, condStm->getElseStm()->getHead(), v0, results);
    if (results.size() < numSols) {
        processCondBranch(currentState, condStm->getThenStm()->getHead(), v1, results);
    }
}

void StateTransitionGraph::processCondBranch(State *currentState, StmNode *nextStm, qc::VectorDD &v,
                                             std::vector<State *> &results) {
    if (!v.isZeroTerminal()) {
        bool inCache = false;
        auto [newState, cache] = makeState(new State(nextStm, v, currentState->stateNr, currentState->depth + 1));
        if (!inCache) {
            currentState->nextStates.push_back(newState->stateNr);
            checkState(newState, results);
        }
    }
}

void StateTransitionGraph::processWhileStm(WhileStmNode *whileStm, State *currentState, StmNode *nextStm,
                                           std::vector<State *> &results) {
    auto *measExp = dynamic_cast<MeasExpNode *>(whileStm->getCond());
    assert(measExp != nullptr);
    auto [v0, v1] = ddSim->measure(measExp, currentState->current);
    processCondBranch(currentState, nextStm, v0, results);
    if (results.size() < numSols) {
        processCondBranch(currentState, whileStm->getBody()->getHead(), v1, results);
    }
}

void StateTransitionGraph::showPath(int stateNr) {
    if (stateNr >= seenStates.size()) {
        std::cout << "Invalid state ID\n";
        return;
    }
    auto *s = seenStates[stateNr];
    if (s->parent != -1) {
        showPath(s->parent);
    }
    std::cout << "[State ID] " << s->stateNr << "\n";
    s->current.printVector<dd::vNode>();
    s->pc->dump(false);
    std::cout << "-------------------\n";
}

std::pair<StateTransitionGraph::State *, bool> StateTransitionGraph::makeState(State *s) {
    auto it = stateTab.find(s);
    if (it != stateTab.end()) {
        std::cout << "Found state in cache, then delete the newly created state\n";
        std::cout << "-------------------\n";
        delete s;
        return {seenStates[it->second], true};
    }
    s->stateNr = seenStates.size();
    stateTab[s] = s->stateNr;
    seenStates.push_back(s);
    return {s, false};
}

void StateTransitionGraph::checkState(State *s, std::vector<State *> &results) {
    if (searchType == SearchType::FINAL_STATES) {
        if (!s->isFinalState())
            return;
    }
    if (ddSim->test(s->current)) {
        results.push_back(s);
    }
}

bool StateTransitionGraph::exist(State *s) {
    auto it = stateTab.find(s);
    return it != stateTab.end();
}

StateTransitionGraph::State *StateTransitionGraph::getNextState() {
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

void StateTransitionGraph::printState(State *s, bool recursive) const {
    if (s == nullptr) {
        return;
    }
    std::cout << "-------------------\n";
    std::cout << "[State ID]: " << s->stateNr << "\n";
    std::cout << "[Parent State ID]: " << s->parent << "\n";
    std::cout << "[Program Counter]: ";
    s->pc->dump(false);
    std::cout << "[Depth]: " << s->depth << "\n";
    std::cout << "[Quantum State]: \n";
    s->current.printVector<dd::vNode>();
    std::cout << "[Next States]: ";
    for (const int i: s->nextStates) {
        std::cout << i << " ";
    }
    std::cout << "\n";
    if (recursive) {
        for (const int i: s->nextStates) {
            printState(seenStates[i]);
        }
    }
}
