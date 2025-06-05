//
// Created by CanhDo on 2024/06/05.
//

#include "core/PStateTransitionGraph.hpp"

#include "ast/CondExpNode.hpp"
#include "ast/NumExpNode.hpp"
#include "core/global.hpp"
#include "utility/Tty.hpp"
#include <Configuration.hpp>
#include <iomanip>
#include <ast/AtomicStmNode.hpp>

PStateTransitionGraph::PStateTransitionGraph(SyntaxProg *currentProg, DDSimulation *ddSim, ExpNode *propExp,
                                             Search::Type type, int numSols, int maxDepth) {
    this->currentProg = currentProg;
    this->ddSim = ddSim;
    this->propExp = propExp;
    this->searchType = type;
    this->numSols = numSols;
    this->depthBound = maxDepth;
    this->ddSim->initProperty(propExp);
}

PStateTransitionGraph::~PStateTransitionGraph() {
    for (auto state: seenStates) {
        delete state;
    }
    seenStates.clear();
    stateTab.clear();
}


void PStateTransitionGraph::buildInitialState() {
    auto *initialState = new State(currentProg->getStmSeq()->getHead(), ddSim->getInitialState());
    assert(seenStates.empty());
    makeState(initialState);
    savedStateId = 0;
}

void PStateTransitionGraph::search() {
    std::cout << "----------- Building state transition graph -----------\n";
    Timer timer(true);
    buildInitialState();
    solutionCount = 0;
    if (searchType == Type::ARROW_STAR) {
        checkState(seenStates.at(0), timer);
    }
    while (solutionCount < numSols && savedStateId < seenStates.size()) {
        // std::cout << "----------- ID: " << savedStateId << " -----------\n";
        State *currentState = seenStates.at(savedStateId);
        if (currentState->depth >= depthBound || (searchType == Type::ARROW_ONE && currentState->depth >= 1)) {
            break;
        }
        procState(currentState, timer);
        savedStateId++;
    }
    // std::cout << "----------- SEARCH END -----------\n";
    gaussSeidelMethod();
    // jacobiMethod();
    printSearchTiming(timer);
    timer.stop();
}

void PStateTransitionGraph::procState(State *currentState, const Timer &timer) {
    StmNode *stm = currentState->pc;
    if (auto *endProg = dynamic_cast<EndStmNode *>(stm); endProg != nullptr && endProg->getNext() == nullptr) {
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
    } else if (auto *atomicStm = dynamic_cast<AtomicStmNode *>(stm); atomicStm != nullptr) {
        procAtomicStm(atomicStm, currentState, nextStm, timer);
    } else {
        throw std::runtime_error("Unknown statement type");
    }
}

StmNode *PStateTransitionGraph::getNextStatement(StmNode *stm) {
    StmNode *nextStm = stm->getNext();
    if (auto *endStm = dynamic_cast<EndStmNode *>(stm->getNext()); endStm != nullptr && endStm->getNext() != nullptr) {
        nextStm = endStm->getNext();
    }
    return nextStm;
}

void PStateTransitionGraph::procSkipStm(SkipStmNode *skipStm, State *currentState, StmNode *nextStm,
                                        const Timer &timer) {
    auto [newState, inCache] = makeState(new State(nextStm, currentState->current, currentState->stateNr,
                                                   currentState->depth + 1));
    currentState->nextStates.push_back(std::make_pair(newState->stateNr, 1));
    if (!inCache) {
        checkState(newState, timer);
    }
}

void PStateTransitionGraph::procUnitaryStm(UnitaryStmNode *unitaryStm, State *currentState, StmNode *nextStm,
                                           const Timer &timer) {
    auto v = currentState->current;
    auto v1 = ddSim->applyGate(unitaryStm, v);
    if (v1.p->ref == 0) {
        ddSim->incRef(v1);
    }
    auto [newState, inCache] = makeState(new State(nextStm, v1, currentState->stateNr, currentState->depth + 1));
    currentState->nextStates.push_back(std::make_pair(newState->stateNr, 1));
    if (!inCache) {
        checkState(newState, timer);
    }
}

void PStateTransitionGraph::procCondStm(CondStmNode *condStm, State *currentState, const Timer &timer) {
    auto *condExp = dynamic_cast<CondExpNode *>(condStm->getCond());
    auto *measExp = dynamic_cast<MeasExpNode *>(condExp->getLeft());
    auto *numExp = dynamic_cast<NumExpNode *>(condExp->getRight());
    assert(condExp != nullptr && measExp != nullptr && numExp != nullptr);
    auto isZero = numExp->isZero();
    auto [v0, pzero, v1, pone] = ddSim->measureWithProb(measExp, currentState->current);
    procCondBranch(currentState, isZero ? condStm->getThenStm()->getHead() : condStm->getElseStm()->getHead(), v0,
                   pzero, 0, timer);
    if (solutionCount < numSols) {
        procCondBranch(currentState, isZero ? condStm->getElseStm()->getHead() : condStm->getThenStm()->getHead(), v1,
                       pone, 1, timer);
    }
}

void PStateTransitionGraph::procCondBranch(State *currentState, StmNode *nextStm, qc::VectorDD &v, qc::fp prob,
                                           int outcome, const Timer &timer) {
    if (prob == 0.0 || v.isZeroTerminal()) {
        return;
    }
    if (v.p->ref == 0) {
        ddSim->incRef(v);
    }
    auto [newState, inCache] = makeState(new StateWithOutcome(nextStm, v, currentState->stateNr,
                                                              currentState->depth + 1, outcome));
    currentState->nextStates.push_back(std::make_pair(newState->stateNr, prob));
    if (!inCache) {
        checkState(newState, timer);
    }
}

void PStateTransitionGraph::procWhileStm(WhileStmNode *whileStm, State *currentState, StmNode *nextStm,
                                         const Timer &timer) {
    auto *condExp = dynamic_cast<CondExpNode *>(whileStm->getCond());
    auto *measExp = dynamic_cast<MeasExpNode *>(condExp->getLeft());
    auto *numExp = dynamic_cast<NumExpNode *>(condExp->getRight());
    assert(condExp != nullptr && measExp != nullptr && numExp != nullptr);
    auto isZero = numExp->isZero();
    auto [v0, pzero, v1, pone] = ddSim->measureWithProb(measExp, currentState->current);
    procCondBranch(currentState, isZero ? whileStm->getBody()->getHead() : nextStm, v0, pzero, 0, timer);
    if (solutionCount < numSols) {
        procCondBranch(currentState, isZero ? nextStm : whileStm->getBody()->getHead(), v1, pone, 1, timer);
    }
}

void PStateTransitionGraph::procAtomicStm(AtomicStmNode *atomicStm, State *currentState, StmNode *nextStm,
                                          const Timer &timer) {
    auto *body = atomicStm->getBody();
    auto *current = body->getHead();
    auto *tail = body->getTail();
    if (current == nullptr || tail == nullptr) {
        return;
    }
    auto v = currentState->current;
    ddSim->incRef(v);
    while (current != tail) {
        if (auto *skipStm = dynamic_cast<SkipStmNode *>(current); skipStm != nullptr) {
            // do nothing, skip statement
        } else if (auto *unitaryStm = dynamic_cast<UnitaryStmNode *>(current); unitaryStm != nullptr) {
            auto v1 = ddSim->applyGate(unitaryStm, v);
            ddSim->decRef(v);
            ddSim->incRef(v1);
            ddSim->garbageCollect();
            v = v1;
        } else {
            throw std::runtime_error("Unknown statement type in atomic statement");
        }
        current = current->getNext();
    }
    ddSim->garbageCollect(true);
    auto [newState, inCache] = makeState(new State(nextStm, v, currentState->stateNr, currentState->depth + 1));
    currentState->nextStates.push_back(std::make_pair(newState->stateNr, 1));
    if (!inCache) {
        checkState(newState, timer);
    }
}

void PStateTransitionGraph::showPath(int stateNr, bool endState) const {
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
    printProbability(s);
    std::cout << ", quantum state:\n";
    s->current.printVector<dd::vNode>();
    if (!endState && !s->isFinalState()) {
        std::cout << "===[ ";
        s->pc->info();
        std::cout << " ]===>\n";
    }
}

std::pair<PStateTransitionGraph::State *, bool> PStateTransitionGraph::makeState(State *s) {
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

void PStateTransitionGraph::checkState(State *s, const Timer &timer) {
    if (searchType == Type::ARROW_EXCLAMATION) {
        if (!s->isFinalState()) {
            probTab[s->stateNr] = 0;
            return;
        }
    }
    if (ddSim->test(s->current, propExp)) {
        targetStates.insert(s->stateNr);
        solutionCount++;
        probTab[s->stateNr] = 1;
    } else {
        probTab[s->stateNr] = 0;
    }
}

const char *PStateTransitionGraph::getSearchType() const {
    return typeNames[static_cast<int>(searchType)];
}

void PStateTransitionGraph::printProbability(State *s) const {
    constexpr auto precision = 3;
    const auto oldPrecision = std::cout.precision();
    std::cout << std::setprecision(precision);
    std::cout << "probability: " << probTab.at(s->stateNr);
    std::cout << std::setprecision(static_cast<int>(oldPrecision));
}

void PStateTransitionGraph::printSearchTiming(State *s, const Timer &timer) const {
    std::cout << "\n";
    std::cout << "Solution " << solutionCount << " (state " << s->stateNr << ")\n";
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
    printProbability(s);
    std::cout << ", quantum state: \n";
    s->current.printVector<dd::vNode>();
}

void PStateTransitionGraph::printSearchTiming(const Timer &timer) const {
    if (solutionCount != 0 && solutionCount >= numSols)
        return;
    std::cout << "\n";
    solutionCount == 0 ? std::cout << "No solution.\n" : std::cout << "No more solutions.\n";
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

void PStateTransitionGraph::printSearchCommand() {
    if (Configuration::systemMode == LOADING_FILE_MODE) {
        std::cout << "==========================================\n";
    }
    std::cout << "psearch in ";
    std::cout << Token::name(currentProg->getName());
    std::cout << " with " << getSearchType() << " such that ";
    propExp->info();
    std::cout << " .\n";
}

void PStateTransitionGraph::dump() const {
    // std::cout << "Initial state: \n";
    // ddSim->getInitialState().printVector<dd::vNode>();
    // std::cout << "Property: \n";
    // propExp->dump();
    // std::cout << "Search type: " << getSearchType() << "\n";
    // std::cout << "Solution bound: ";
    // if (numSols == UNBOUNDED) {
    //     std::cout << "unbounded\n";
    // } else {
    //     std::cout << numSols << "\n";
    // }
    // std::cout << "Depth bound: ";
    // if (depthBound == UNBOUNDED) {
    //     std::cout << "unbounded\n";
    // } else {
    //     std::cout << depthBound << "\n";
    // }
    for(auto &prob : probTab) {
        std::cout << "State ID: " << prob.first << ", Probability: " << prob.second << "\n";
    }
    // std::cout << "State Transition Graph\n";
    // std::cout << "State ID being considered: " << savedStateId << "\n";
    // std::cout << "-------------------\n";
    // if (seenStates.size() != 0) {
    //     printState(seenStates[0]);
    // }
}

void PStateTransitionGraph::printState(State *s, bool recursive) const {
    if (s == nullptr) {
        return;
    }
    std::cout << "-------------------\n";
    std::cout << "[State ID]: " << s->stateNr << "\n";
    std::cout << "[Parent State ID]: " << s->parent << "\n";
    std::cout << "[Program Counter]: ";
    s->pc->dump(false);
    std::cout << "[Depth]: " << s->depth << "\n";
    std::cout << "[Probability]: " << probTab.at(s->stateNr) << "\n";
    std::cout << "[Quantum State]: \n";
    s->current.printVector<dd::vNode>();
    std::cout << "[Next States]: ";
    for (auto [i, prob]: s->nextStates) {
        std::cout << i << ", " << prob << " ";
    }
    std::cout << "\n";
    if (recursive) {
        for (auto [i, prob] : s->nextStates) {
            if (i <= s->stateNr) {
                // do not print backward states
                std::cout << "Backward state detected\n";
                continue;
            };
            printState(seenStates.at(i));
        }
    }
}

void PStateTransitionGraph::gaussSeidelMethod(int maxIter, qc::fp tol) {
    std::cout << "Running Gauss-Seidel method for probabilistic state transition graph...\n";
    for (int i = 0; i < maxIter; i++) {
        qc::fp maxDiff = 0.0;
        for (auto state: seenStates) {
            if (targetStates.find(state->stateNr) != targetStates.end()) {
                continue;
            }
            qc::fp newProb = 0.0;
            qc::fp oldProb = probTab[state->stateNr];
            for (const auto &[nextStateId, prob]: state->nextStates) {
                newProb += prob * probTab[nextStateId];
            }
            if (newProb > 1.0) {
                newProb = 1.0;
            }
            probTab[state->stateNr] = newProb;
            maxDiff = std::max(maxDiff, std::abs(newProb - oldProb));
        }
        if (maxDiff < tol) {
            std::cout << "Gauss-Seidel method converged after " << i + 1 << " iterations.\n";
            break;
        }
    }
    std::cout << "Result: " << probTab[0] << std::endl;
}

void PStateTransitionGraph::jacobiMethod(int maxIter, qc::fp tol) {
    std::cout << "Running Jacobi method for probabilistic state transition graph...\n";
    for (int i = 0; i < maxIter; i++) {
        qc::fp maxDiff = 0.0;
        std::unordered_map<int, qc::fp> newProbTab;
        for (auto state: seenStates) {
            if (targetStates.find(state->stateNr) != targetStates.end()) {
                newProbTab[state->stateNr] = 1.0;
                continue;
            }
            qc::fp newProb = 0.0;
            for (const auto &[nextStateId, prob]: state->nextStates) {
                newProb += prob * probTab[nextStateId];
            }
            if (newProb > 1.0) {
                newProb = 1.0;
            }
            newProbTab[state->stateNr] = newProb;
            maxDiff = std::max(maxDiff, std::abs(newProb - probTab[state->stateNr]));
        }
        probTab = std::move(newProbTab);
        if (maxDiff < tol) {
            std::cout << "Jacobi method converged after " << i + 1 << " iterations.\n";
            break;
        }
    }
    std::cout << "Result: " << probTab[0] << std::endl;
}
