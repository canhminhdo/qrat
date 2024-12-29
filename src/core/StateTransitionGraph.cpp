//
// Created by CanhDo on 2024/12/18.
//

#include "core/StateTransitionGraph.hpp"

#include "ast/CondExpNode.hpp"
#include "ast/NumExpNode.hpp"

StateTransitionGraph::StateTransitionGraph(SyntaxProg *currentProg, DDSimulation *ddSim, ExpNode *propExp, Search::Type type, int numSols, int maxDepth) {
    this->currentProg = currentProg;
    this->ddSim = ddSim;
    this->propExp = propExp;
    this->searchType = type;
    this->numSols = numSols;
    this->depthBound = maxDepth;
    this->ddSim->initProperty(propExp);
}

void StateTransitionGraph::buildInitialState() {
    auto *initialState = new State(currentProg->getStmSeq()->getHead(), ddSim->getInitialState());
    assert(seenStates.empty());
    makeState(initialState);
    savedStateId = 0;
}

void StateTransitionGraph::search() {
    buildInitialState();
    std::unordered_set<int> results;
    if (searchType == Type::ARROW_STAR) {
        checkState(seenStates[0], results);
    }
    while (results.size() < numSols && savedStateId < seenStates.size()) {
        State *currentState = seenStates[savedStateId];
        if (currentState->depth >= depthBound || (searchType == Type::ARROW_ONE && currentState->depth >= 1)) {
            break;
        }
        procState(currentState, results);
        savedStateId++;
    }
    std::cout << "----------- SEARCH SUMMARY -----------\n";
    std::cout << "#Solutions: " << results.size() << "\n";
    for (auto stateNr: results) {
        printState(seenStates[stateNr], false);
        // std::cout << "----------- S-Show path: " << stateNr << " -----------\n";
        // showPath(stateNr);
        // std::cout << "----------- E-Show path: " << stateNr << " -----------\n";
    }
}

void StateTransitionGraph::procState(State *currentState, std::unordered_set<int> &results) {
    StmNode *stm = currentState->pc;
    if (auto *endProg = dynamic_cast<EndStmNode *>(stm); endProg != nullptr && endProg->getNext() == nullptr) {
        return;
    }
    StmNode *nextStm = getNextStatement(stm);
    if (auto *skipStm = dynamic_cast<SkipStmNode *>(stm); skipStm != nullptr) {
        procSkipStm(skipStm, currentState, nextStm, results);
    } else if (auto *unitaryStm = dynamic_cast<UnitaryStmNode *>(stm); unitaryStm != nullptr) {
        procUnitaryStm(unitaryStm, currentState, nextStm, results);
    } else if (auto *condStm = dynamic_cast<CondStmNode *>(stm); condStm != nullptr) {
        procCondStm(condStm, currentState, results);
    } else if (auto *whileStm = dynamic_cast<WhileStmNode *>(stm); whileStm != nullptr) {
        procWhileStm(whileStm, currentState, nextStm, results);
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

void StateTransitionGraph::procSkipStm(SkipStmNode *skipStm, State *currentState, StmNode *nextStm, std::unordered_set<int> &results) {
    auto [newState, inCache] = makeState(new State(nextStm, currentState->current, currentState->stateNr,
                                                   currentState->depth + 1, currentState->prob));
    currentState->nextStates.push_back(newState->stateNr);
    if (!inCache) {
        checkState(newState, results);
    }
}

void StateTransitionGraph::procUnitaryStm(UnitaryStmNode *unitaryStm, State *currentState, StmNode *nextStm,
                                             std::unordered_set<int> &results) {
    auto v = currentState->current;
    auto v1 = ddSim->applyGate(unitaryStm, v);
    auto [newState, inCache] = makeState(new State(nextStm, v1, currentState->stateNr, currentState->depth + 1, currentState->prob));
    currentState->nextStates.push_back(newState->stateNr);
    if (!inCache) {
        checkState(newState, results);
    }
}

void StateTransitionGraph::procCondStm(CondStmNode *condStm, State *currentState, std::unordered_set<int> &results) {
    auto *condExp = dynamic_cast<CondExpNode *>(condStm->getCond());
    auto *measExp = dynamic_cast<MeasExpNode *>(condExp->getLeft());
    auto *numExp = dynamic_cast<NumExpNode *>(condExp->getRight());
    assert(condExp != nullptr && measExp != nullptr && numExp != nullptr);
    auto isZero = numExp->isZero();
    auto [v0, pzero, v1, pone] = ddSim->measureWithProb(measExp, currentState->current);

    procCondBranch(currentState, isZero ? condStm->getThenStm()->getHead() : condStm->getElseStm()->getHead(), v0, pzero, results);
    if (results.size() < numSols) {
        procCondBranch(currentState, isZero ? condStm->getElseStm()->getHead() : condStm->getThenStm()->getHead(), v1, pone, results);
    }
}

void StateTransitionGraph::procCondBranch(State *currentState, StmNode *nextStm, qc::VectorDD &v, qc::fp prob,
                                             std::unordered_set<int> &results) {
    if (!v.isZeroTerminal()) {
        bool inCache = false;
        auto [newState, cache] = makeState(new State(nextStm, v, currentState->stateNr, currentState->depth + 1, currentState->prob * prob));
        currentState->nextStates.push_back(newState->stateNr);
        if (!inCache) {
            checkState(newState, results);
        }
    }
}

void StateTransitionGraph::procWhileStm(WhileStmNode *whileStm, State *currentState, StmNode *nextStm,
                                           std::unordered_set<int> &results) {
    auto *condExp = dynamic_cast<CondExpNode *>(whileStm->getCond());
    auto *measExp = dynamic_cast<MeasExpNode *>(condExp->getLeft());
    auto *numExp = dynamic_cast<NumExpNode *>(condExp->getRight());
    assert(condExp != nullptr && measExp != nullptr && numExp != nullptr);
    auto isZero = numExp->isZero();
    auto [v0, pzero, v1, pone] = ddSim->measureWithProb(measExp, currentState->current);
    procCondBranch(currentState, isZero ? whileStm->getBody()->getHead() : nextStm, v0, pzero, results);
    if (results.size() < numSols) {
        procCondBranch(currentState, isZero ? nextStm : whileStm->getBody()->getHead(), v1, pone, results);
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
        delete s;
        return {seenStates[it->second], true};
    }
    s->stateNr = seenStates.size();
    stateTab[s] = s->stateNr;
    seenStates.push_back(s);
    return {s, false};
}

void StateTransitionGraph::checkState(State *s, std::unordered_set<int> &results) {
    if (searchType == Type::ARROW_EXCLAMATION) {
        if (!s->isFinalState())
            return;
    }
    if (ddSim->test(s->current, propExp)) {
        results.insert(s->stateNr);
    }
}

const char* StateTransitionGraph::getSearchType() const {
    return typeNames[static_cast<int>(searchType)];
}

void StateTransitionGraph::dump() const {
    std::cout << "Initial state: \n";
    ddSim->getInitialState().printVector<dd::vNode>();
    std::cout << "Property: \n";
    propExp->dump();
    std::cout << "Search type: " << getSearchType() << "\n";
    std::cout << "Solution bound: ";
    if (numSols == UNBOUNDED) {
        std::cout << "unbounded\n";
    } else {
        std::cout << numSols << "\n";
    }
    std::cout << "Depth bound: ";
    if (depthBound == UNBOUNDED) {
        std::cout << "unbounded\n";
    } else {
        std::cout << depthBound << "\n";
    }
    // std::cout << "State Transition Graph\n";
    // std::cout << "State ID being considered: " << savedStateId << "\n";
    // std::cout << "-------------------\n";
    // if (seenStates.size() != 0) {
    //     printState(seenStates[0]);
    // }
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
    std::cout << "[Probability]: " << s->prob << "\n";
    std::cout << "[Quantum State]: \n";
    s->current.printVector<dd::vNode>();
    std::cout << "[Next States]: ";
    for (const int i: s->nextStates) {
        std::cout << i << " ";
    }
    std::cout << "\n";
    if (recursive) {
        for (const int i: s->nextStates) {
            if (i <= s->stateNr) {
                // do not print backward states
                std::cout << "Backward state detected\n";
                continue;
            };
            printState(seenStates[i]);
        }
    }
}
