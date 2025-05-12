//
// Created by CanhDo on 2024/12/18.
//

#include "core/StateTransitionGraph2.hpp"

#include "ast/CondExpNode.hpp"
#include "ast/NumExpNode.hpp"
#include "core/global.hpp"
#include "utility/Tty.hpp"
#include <Configuration.hpp>
#include <iomanip>
#include <unistd.h>

StateTransitionGraph2::StateTransitionGraph2(SyntaxProg *currentProg, DDSimulation *ddSim, char *property) {
    this->currentProg = currentProg;
    this->ddSim = ddSim;
    this->property = property;
    this->ddSim->initProperty2();
    for (auto &prop: currentProg->getPropTab().getPropTab()) {
        prop2States[prop.first] = std::vector<int>();
    }
}

StateTransitionGraph2::~StateTransitionGraph2() {
    for (auto *state: seenStates) {
        delete state;
    }
    seenStates.clear();
    stateTab.clear();
}


void StateTransitionGraph2::buildInitialState() {
    auto *initialState = new State(currentProg->getStmSeq()->getHead(), ddSim->getInitialState());
    assert(seenStates.empty());
    makeState(initialState);
    savedStateId = 0;
}

void StateTransitionGraph2::search() {
    // std::cout << "----------- SEARCH START -----------\n";
    Timer timer(true);
    buildInitialState();
    checkState(seenStates[0]);
    while (savedStateId < seenStates.size()) {
        // std::cout << "----------- ID: " << savedStateId << " -----------\n";
        State *currentState = seenStates[savedStateId];
        procState(currentState, timer);
        savedStateId++;
    }
    // std::cout << "----------- SEARCH END -----------\n";
    printSearchTiming(timer);
    timer.stop();
    // dump();
}

void StateTransitionGraph2::procState(State *currentState, const Timer &timer) {
    StmNode *stm = currentState->pc;
    if (auto *endProg = dynamic_cast<EndStmNode *>(stm); endProg != nullptr && endProg->getNext() == nullptr) {
        ending.push_back(currentState->stateNr);
        return;
    }
    StmNode *nextStm = getNextStatement(stm);
    if (auto *skipStm = dynamic_cast<SkipStmNode *>(stm); skipStm != nullptr) {
        procSkipStm(skipStm, currentState, nextStm, timer);
    } else if (auto *unitaryStm = dynamic_cast<UnitaryStmNode *>(stm); unitaryStm != nullptr) {
        procUnitaryStm(unitaryStm, currentState, nextStm, timer);
    } else if (auto *condStm = dynamic_cast<CondStmNode *>(stm); condStm != nullptr) {
        procCondStm(condStm, currentState, timer);
    } else if (auto *whileStm = dynamic_cast<WhileStmNode *>(stm); whileStm != nullptr) {
        procWhileStm(whileStm, currentState, nextStm, timer);
    } else {
        throw std::runtime_error("Unknown statement type");
    }
}

StmNode *StateTransitionGraph2::getNextStatement(StmNode *stm) {
    StmNode *nextStm = stm->getNext();
    if (auto *endStm = dynamic_cast<EndStmNode *>(stm->getNext()); endStm != nullptr && endStm->getNext() != nullptr) {
        nextStm = endStm->getNext();
    }
    return nextStm;
}

void StateTransitionGraph2::procSkipStm(SkipStmNode *skipStm, State *currentState, StmNode *nextStm,
                                        const Timer &timer) {
    auto [newState, inCache] = makeState(new State(nextStm, currentState->current, currentState->stateNr, currentState->depth + 1));
    currentState->nextStates.push_back(std::make_pair(newState->stateNr, 1));
    if (!inCache) {
        checkState(newState);
    }
}

void StateTransitionGraph2::procUnitaryStm(UnitaryStmNode *unitaryStm, State *currentState, StmNode *nextStm,
                                           const Timer &timer) {
    auto v = currentState->current;
    auto v1 = ddSim->applyGate(unitaryStm, v);
    auto [newState, inCache] = makeState(new State(nextStm, v1, currentState->stateNr, currentState->depth + 1));
    currentState->nextStates.push_back(std::make_pair(newState->stateNr, 1));
    if (!inCache) {
        checkState(newState);
    }
}

void StateTransitionGraph2::procCondStm(CondStmNode *condStm, State *currentState, const Timer &timer) {
    auto *condExp = dynamic_cast<CondExpNode *>(condStm->getCond());
    auto *measExp = dynamic_cast<MeasExpNode *>(condExp->getLeft());
    auto *numExp = dynamic_cast<NumExpNode *>(condExp->getRight());
    assert(condExp != nullptr && measExp != nullptr && numExp != nullptr);
    auto isZero = numExp->isZero();
    auto [v0, pzero, v1, pone] = ddSim->measureWithProb(measExp, currentState->current);

    procCondBranch(currentState, isZero ? condStm->getThenStm()->getHead() : condStm->getElseStm()->getHead(), v0,
                   pzero, 0, timer);
    procCondBranch(currentState, isZero ? condStm->getElseStm()->getHead() : condStm->getThenStm()->getHead(), v1,
                   pone, 1, timer);
}

void StateTransitionGraph2::procCondBranch(State *currentState, StmNode *nextStm, qc::VectorDD &v, qc::fp prob,
                                           int outcome, const Timer &timer) {
    if (prob == 0.0 || v.isZeroTerminal()) {
        return;
    }
    auto [newState, inCache] = makeState(new StateWithOutcome(nextStm, v, currentState->stateNr, currentState->depth + 1, outcome));
    currentState->nextStates.push_back(std::make_pair(newState->stateNr, prob));
    if (!inCache) {
        checkState(newState);
    }
}

void StateTransitionGraph2::procWhileStm(WhileStmNode *whileStm, State *currentState, StmNode *nextStm,
                                         const Timer &timer) {
    auto *condExp = dynamic_cast<CondExpNode *>(whileStm->getCond());
    auto *measExp = dynamic_cast<MeasExpNode *>(condExp->getLeft());
    auto *numExp = dynamic_cast<NumExpNode *>(condExp->getRight());
    assert(condExp != nullptr && measExp != nullptr && numExp != nullptr);
    auto isZero = numExp->isZero();
    auto [v0, pzero, v1, pone] = ddSim->measureWithProb(measExp, currentState->current);
    procCondBranch(currentState, isZero ? whileStm->getBody()->getHead() : nextStm, v0, pzero, 0, timer);
    procCondBranch(currentState, isZero ? nextStm : whileStm->getBody()->getHead(), v1, pone, 1, timer);
}

std::pair<StateTransitionGraph2::State *, bool> StateTransitionGraph2::makeState(State *s) {
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

void StateTransitionGraph2::checkState(State *s) {
    for (auto &prop: currentProg->getPropTab().getPropTab()) {
        auto *propExp = prop.second;
        if (ddSim->test(s->current, propExp)) {
            prop2States[prop.first].push_back(s->stateNr);
        }
    }
}

void StateTransitionGraph2::printSearchTiming(const Timer &timer) const {
    std::cout << "states: " << seenStates.size();
    if (Configuration::showTiming) {
        Int64 real;
        Int64 virt;
        Int64 prof;
        if (timer.getTimes(real, virt, prof)) {
            std::cout << " in " << prof / 1000 << "ms cpu (" << real / 1000 << "ms real)";
        }
    }
    std::cout << "\n";
}

void StateTransitionGraph2::printSearchCommand() {
    if (Configuration::systemMode == LOADING_FILE_MODE) {
        std::cout << "==========================================\n";
    }
    std::cout << "pmc in ";
    std::cout << Token::name(currentProg->getName());
    std::cout << " with " << property << "";
    std::cout << " .\n";
}

void StateTransitionGraph2::dump() const {
    std::cout << "Initial state: \n";
    ddSim->getInitialState().printVector<dd::vNode>();
    std::cout << "State Transition Graph\n";
    std::cout << "State ID being considered: " << savedStateId << "\n";
    std::cout << "-------------------\n";
    if (seenStates.size() != 0) {
        printState(seenStates[0]);
    }
}

void StateTransitionGraph2::printState(State *s, bool recursive) const {
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
    for (const auto &[i, prob]: s->nextStates) {
        std::cout << i << " : " << prob << "\n";
    }
    std::cout << "\n";
    if (recursive) {
        for (const auto &[i, prob]: s->nextStates) {
            if (i <= s->stateNr) {
                // do not print backward states
                std::cout << "Backward state detected\n";
                continue;
            };
            printState(seenStates[i]);
        }
    }
}

std::vector<StateTransitionGraph2::State *> &StateTransitionGraph2::getSeenStates() {
    return seenStates;
}

std::unordered_map<int, std::vector<int>> &StateTransitionGraph2::getProp2States() {
    return prop2States;
}

std::vector<int> &StateTransitionGraph2::getEndingStates() {
    return ending;
}

char *StateTransitionGraph2::getProperty() {
    return property;
}
