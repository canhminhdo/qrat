//
// Created by CanhDo on 2025/06/07.
//

#include "core/SearchGraph.hpp"
#include "core/global.hpp"
#include "utility/Tty.hpp"
#include <Configuration.hpp>
#include <iomanip>

SearchGraph::~SearchGraph() {
    for (auto state: seenStates) {
        delete state;
    }
    seenStates.clear();
    stateTab.clear();
}

void SearchGraph::buildInitialState() {
    auto *initialState = new State(getCfg().entry(), ddSim->getInitialState());
    assert(seenStates.empty());
    makeState(initialState);
}

void SearchGraph::handleInCache(int currStateId, int nextStateId) {
}

void SearchGraph::procState(State *currentState, const Timer &timer) {
    const auto &loc = getCfg().at(currentState->pc);
    switch (loc.kind) {
        case ControlFlowGraph::Kind::HALT:
            return;
        case ControlFlowGraph::Kind::SKIP:
            procSkipStm(loc, currentState, timer);
            break;
        case ControlFlowGraph::Kind::UNITARY:
            procUnitaryStm(loc, currentState, timer);
            break;
        case ControlFlowGraph::Kind::BRANCH:
            procBranchStm(loc, currentState, timer);
            break;
        case ControlFlowGraph::Kind::ATOMIC:
            procAtomicStm(loc, currentState, timer);
            break;
    }
}

void SearchGraph::procSkipStm(const ControlFlowGraph::Location &loc, State *currentState, const Timer &timer) {
    auto [newState, inCache] = makeState(new State(loc.next[0], currentState->current, currentState->stateNr,
                                                   currentState->depth + 1));
    currentState->nextStates.push_back(std::make_pair(newState->stateNr, 1));
    if (!inCache) {
        checkState(newState, timer);
    } else {
        handleInCache(currentState->stateNr, newState->stateNr);
    }
}

void SearchGraph::procUnitaryStm(const ControlFlowGraph::Location &loc, State *currentState, const Timer &timer) {
    auto v = currentState->current;
    auto v1 = ddSim->applyGate(loc.gate, v);
    if (ddSim->isUnreferenced(v1)) {
        ddSim->incRef(v1);
    }
    auto [newState, inCache] = makeState(new State(loc.next[0], v1, currentState->stateNr, currentState->depth + 1));
    currentState->nextStates.push_back(std::make_pair(newState->stateNr, 1));
    if (!inCache) {
        checkState(newState, timer);
    } else {
        handleInCache(currentState->stateNr, newState->stateNr);
    }
}

void SearchGraph::procBranchStm(const ControlFlowGraph::Location &loc, State *currentState, const Timer &timer) {
    auto [v0, pZero, v1, pOne] = ddSim->measureWithProb(loc.meas, currentState->current);
    procCondBranch(currentState, loc.next[1], v1, pOne, 1, timer);
    if (checkSearchCondition()) {
        procCondBranch(currentState, loc.next[0], v0, pZero, 0, timer);
    }
}

void SearchGraph::procAtomicStm(const ControlFlowGraph::Location &loc, State *currentState, const Timer &timer) {
    auto v = currentState->current;
    ddSim->incRef(v);
    for (auto *unitaryStm: loc.gates) {
        auto v1 = ddSim->applyGate(unitaryStm, v);
        ddSim->decRef(v);
        ddSim->incRef(v1);
        ddSim->garbageCollect();
        v = v1;
    }
    ddSim->garbageCollect(true);
    auto [newState, inCache] = makeState(new State(loc.next[0], v, currentState->stateNr, currentState->depth + 1));
    currentState->nextStates.push_back(std::make_pair(newState->stateNr, 1));
    if (!inCache) {
        checkState(newState, timer);
    } else {
        handleInCache(currentState->stateNr, newState->stateNr);
    }
}

void SearchGraph::procCondBranch(State *currentState, int nextLoc, QState &v, const Prob &prob, int outcome, const Timer &timer) {
    if (prob.isZero() || v.isZeroTerminal()) {
        return;
    }
    if (ddSim->isUnreferenced(v)) {
        ddSim->incRef(v);
    }
    auto [newState, inCache] = makeState(new StateWithOutcome(nextLoc, v, currentState->stateNr,
                                                              currentState->depth + 1, outcome));
    currentState->nextStates.push_back(std::make_pair(newState->stateNr, prob));
    if (!inCache) {
        checkState(newState, timer);
    } else {
        handleInCache(currentState->stateNr, newState->stateNr);
    }
}

bool SearchGraph::isFinalState(const State *s) const {
    return s->pc == getCfg().halt();
}

const ControlFlowGraph &SearchGraph::getCfg() const {
    return currentProg->getCfg();
}

void SearchGraph::showPath(int stateNr, bool endState) const {
    if (stateNr >= seenStates.size()) {
        std::cout << Tty(Tty::RED) << "Error: State ID is invalid" << Tty(Tty::RESET) << std::endl;
        return;
    }
    auto s = seenStates.at(stateNr);
    if (s->parent != -1) {
        showPath(s->parent, false);
    }
    if (s->hasOutcome()) {
        std::cout << "(with measurement result: " << s->getOutcome() << ")\n";
    }
    std::cout << "state " << s->stateNr << ", ";
    std::cout << "quantum state:\n";
    ddSim->printState(s->current);
    if (!endState && !isFinalState(s)) {
        std::cout << "===[ ";
        getCfg().at(s->pc).stm->info();
        std::cout << " ]===>\n";
    }
}

void SearchGraph::showState(int stateNr) const {
    if (stateNr >= seenStates.size()) {
        std::cout << Tty(Tty::RED) << "Error: State ID is invalid" << Tty(Tty::RESET) << std::endl;
        return;
    }
    auto s = seenStates.at(stateNr);
    if (s == nullptr) {
        return;
    }
    std::cout << "state " << s->stateNr << ", ";
    std::cout << "quantum state:\n";
    ddSim->printState(s->current);
}

void SearchGraph::showBasisInfo(int stateNr, std::string basis, bool isProb) const {
    if (stateNr >= seenStates.size()) {
        std::cout << Tty(Tty::RED) << "Error: State ID is invalid" << Tty(Tty::RESET) << std::endl;
        return;
    }
    auto s = seenStates.at(stateNr);
    if (s == nullptr) {
        return;
    }
    if (isProb) {
        std::cout << Tty(Tty::YELLOW) << "Probability of basis state |" << basis << "> in state " << stateNr << " is "<< ddSim->basisProb(s->current, basis) << Tty(Tty::RESET) << std::endl;
    } else {
        std::cout << Tty(Tty::YELLOW) << "Amplitude of basis state |" << basis << "> in state " << stateNr << " is "<< ddSim->basisAmplitude(s->current, basis) << Tty(Tty::RESET) << std::endl;
    }
}

void SearchGraph::printState(State *s, bool recursive) const {
    if (s == nullptr) {
        return;
    }
    std::cout << "-------------------\n";
    std::cout << "[State ID]: " << s->stateNr << "\n";
    std::cout << "[Parent State ID]: " << s->parent << "\n";
    std::cout << "[Program Counter]: ";
    if (isFinalState(s)) {
        std::cout << "End" << std::endl;
    } else {
        getCfg().at(s->pc).stm->dump(false);
    }
    std::cout << "[Depth]: " << s->depth << "\n";
    std::cout << "[Quantum State]: \n";
    ddSim->printState(s->current);
    std::cout << "[Next States]: ";
    for (auto [i, prob] : s->nextStates) {
        std::cout << i << " ";
    }
    std::cout << "\n";
    if (recursive) {
        for (auto [i, prob]: s->nextStates) {
            if (i <= s->stateNr) {
                // do not print backward states
                std::cout << "Backward state detected\n";
                continue;
            };
            printState(seenStates.at(i));
        }
    }
}

std::pair<SearchGraph::State *, bool> SearchGraph::makeState(State *s) {
    auto it = stateTab.find(s);
    if (it != stateTab.end()) {
        // std::cout << "Found state in cache, then delete the newly created state\n";
        delete s;
        return {seenStates[it->second], true};
    }
    s->stateNr = seenStates.size();
    stateTab[s] = s->stateNr;
    seenStates.push_back(s);
    return {s, false};
}

std::vector<SearchGraph::State *> &SearchGraph::getSeenStates() {
    return seenStates;
}

const SyntaxProg *SearchGraph::getProg() const {
    return currentProg;
}
