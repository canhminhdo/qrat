//
// Created by Canh Minh Do on 2025/06/08.
//

#include "core/StateSpaceGraph.hpp"

#include <Configuration.hpp>

StateSpaceGraph::StateSpaceGraph(SyntaxProg *currentProg, DDSimulation *ddSim, char *property) {
    this->currentProg = currentProg;
    this->ddSim = ddSim;
    this->property = property;
    this->ddSim->initProperty2();
    for (auto &prop: currentProg->getPropTab().getPropTab()) {
        prop2States[prop.first] = std::vector<int>();
    }
}

void StateSpaceGraph::search() {
    // std::cout << "----------- SEARCH START -----------\n";
    Timer timer(true);
    buildInitialState();
    checkState(seenStates[0], timer);
    int savedStateId = 0;
    while (savedStateId < seenStates.size()) {
        // std::cout << "----------- ID: " << savedStateId << " -----------\n";
        State *currentState = seenStates[savedStateId];
        procState(currentState, timer);
        savedStateId++;
    }
    // std::cout << "----------- SEARCH END -----------\n";
    printExploredStates(timer);
    timer.stop();
    // dump();
}

bool StateSpaceGraph::checkSearchCondition() {
    std::cout << "Checking search condition in StateSpaceGraph...\n";
    return true;
}

void StateSpaceGraph::checkState(State *s, const Timer &timer) {
    for (auto &prop: currentProg->getPropTab().getPropTab()) {
        auto *propExp = prop.second;
        if (ddSim->test(s->current, propExp)) {
            prop2States[prop.first].push_back(s->stateNr);
        }
    }
}

void StateSpaceGraph::printExploredStates(const Timer &timer) const {
    std::cout << "\n";
    std::cout << "Time for state transition graph construction with states: " << seenStates.size();
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

void StateSpaceGraph::printCommand() {
    if (Configuration::systemMode == LOADING_FILE_MODE) {
        std::cout << "==========================================\n";
    }
    std::cout << "pcheck in ";
    std::cout << Token::name(currentProg->getName());
    std::cout << " with " << property << "";
    std::cout << " .\n";
}

void StateSpaceGraph::dump() const {
    std::cout << "Initial state: \n";
    ddSim->getInitialState().printVector<dd::vNode>();
    std::cout << "State Transition Graph\n";
    std::cout << "-------------------\n";
    if (seenStates.size() != 0) {
        printState(seenStates[0]);
    }
}

std::unordered_map<int, std::vector<int>> &StateSpaceGraph::getProp2States() {
    return prop2States;
}

std::vector<int> &StateSpaceGraph::getEndingStates() {
    return ending;
}

char *StateSpaceGraph::getProperty() {
    return property;
}