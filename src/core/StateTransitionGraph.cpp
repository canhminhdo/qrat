//
// Created by CanhDo on 2024/12/18.
//

#include "core/StateTransitionGraph.hpp"
#include "ast/CondExpNode.hpp"
#include "core/global.hpp"
#include "utility/Tty.hpp"
#include "Configuration.hpp"
#include<queue>

StateTransitionGraph::StateTransitionGraph(SyntaxProg *currentProg, DDSimulation *ddSim, ExpNode *propExp,
                                           Search::Type type, int numSols, int maxDepth, bool probMode) {
    this->currentProg = currentProg;
    this->ddSim = ddSim;
    this->propExp = propExp;
    this->searchType = type;
    this->numSols = numSols;
    this->depthBound = maxDepth;
    this->probMod = probMode;
    this->ddSim->initProperty(propExp);
}

void StateTransitionGraph::handleInCache(int currStateId, int nextStateId) {
    if (probMod) {
        seenStates.at(nextStateId)->otherParents.push_back(currStateId);
    }
}

void StateTransitionGraph::search() {
    if (probMod) {
        DEBUG(
            std::cout << "Building state transition graph ...\n";
        )
    }
    Timer timer(true);
    buildInitialState();
    solutionCount = 0;
    int savedStateId = 0;
    if (isArrowStar(searchType)) {
        checkState(seenStates.at(0), timer);
    }
    while (solutionCount < numSols && savedStateId < seenStates.size()) {
        State *currentState = seenStates.at(savedStateId);
        if (currentState->depth >= depthBound || (isArrowOne(searchType) && currentState->depth >= 1)) {
            break;
        }
        procState(currentState, timer);
        savedStateId++;
    }
    if (probMod) gaussSeidelMethod();
    printExploredStates(timer);
    timer.stop();
}

bool StateTransitionGraph::checkSearchCondition() {
    return solutionCount < numSols;
}

void StateTransitionGraph::checkState(State *s, const Timer &timer) {
    if (isArrowExclamation(searchType)) {
        if (!s->isFinalState())
            return;
    }
    if (ddSim->test(s->current, propExp)) {
        solutionCount++;
        if (probMod) {
            targetStates.insert(s->stateNr);
        } else {
            printSearchTiming(s, timer);
        }
    }
}

void StateTransitionGraph::printExploredStates(const Timer &timer) const {
    if (solutionCount != 0 && solutionCount >= numSols)
        return;
    std::cout << "\n";
    if (!probMod) {
        solutionCount == 0 ? std::cout << "No solution.\n" : std::cout << "No more solutions.\n";
    }
    std::cout << "states: " << seenStates.size();
    if (Configuration::showTiming) {
        Int64 real;
        Int64 virt;
        Int64 prof;
        if (timer.getTimes(real, virt, prof)) {
            std::cout << " in " << prof / 1000 << "ms cpu (" << real / 1000 << "ms real)";
        }
    }
    std::cout << std::endl;
}

void StateTransitionGraph::printCommand() {
    if (Configuration::systemMode == LOADING_FILE_MODE) {
        std::cout << "==========================================\n";
    }
    std::cout << (probMod ? "psearch in " : "search in ");
    std::cout << Token::name(currentProg->getName());
    std::cout << " with " << getSearchType(searchType) << " such that ";
    propExp->info();
    std::cout << " ." << std::endl;
}

void StateTransitionGraph::printSearchTiming(State *s, const Timer &timer) const {
    if (probMod)
        return;
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
    std::cout << "quantum state: \n";
    s->current.printVector<dd::vNode>();
    std::cout.flush();
}

void StateTransitionGraph::dump() const {
    std::cout << "Initial state: \n";
    ddSim->getInitialState().printVector<dd::vNode>();
    std::cout << "Property: \n";
    propExp->dump();
    std::cout << "Search type: " << getSearchType(searchType) << "\n";
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
    // std::cout << "-------------------\n";
    // if (seenStates.size() != 0) {
    //     printState(seenStates[0]);
    // }
}

void StateTransitionGraph::gaussSeidelMethod(int maxIter, qc::fp tol) {
    DEBUG(
        std::cout << "Running Gauss-Seidel method for probabilistic state transition graph...\n";
    );
    auto backwardStates = backwardReachable();
    for (int i = 0; i < maxIter; i++) {
        qc::fp maxDiff = 0.0;
        for (auto stateNr: backwardStates) {
            auto state = seenStates.at(stateNr);
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
            DEBUG(
                std::cout << "Gauss-Seidel method converged after " << i + 1 << " iterations.\n";
            );
            break;
        }
    }
    std::cout << "\nResult: " << probTab[0] << std::endl;
}

void StateTransitionGraph::jacobiMethod(int maxIter, qc::fp tol) {
    DEBUG(
        std::cout << "Running Jacobi method for probabilistic state transition graph...\n";
    );
    auto backwardStates = backwardReachable();
    for (int i = 0; i < maxIter; i++) {
        qc::fp maxDiff = 0.0;
        std::unordered_map<int, qc::fp> newProbTab;
        for (auto stateNr: backwardStates) {
            auto state = seenStates.at(stateNr);
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
            DEBUG(
                std::cout << "Jacobi method converged after " << i + 1 << " iterations.\n";
            );
            break;
        }
    }
    std::cout << "\nResult: " << probTab[0] << std::endl;
}

std::unordered_set<int> StateTransitionGraph::backwardReachable() {
    std::unordered_set<int> visited;
    std::queue<int> q;
    for (auto id: targetStates) {
        visited.insert(id);
        q.push(id);
        probTab[id] = 1.0;
    }
    int count = targetStates.size();
    while (!q.empty()) {
        int stateId = q.front();
        q.pop();
        if (count == 0) {
            probTab[stateId] = 0.0;
        } else {
            count--;
        }
        auto state = seenStates.at(stateId);
        auto processParent = [&](int parentId) {
            if (parentId != -1 && visited.insert(parentId).second) {
                q.push(parentId);
            }
        };
        processParent(state->parent);
        for (auto otherParentId : state->otherParents) {
            processParent(otherParentId);
        }
    }
    return visited;
}
