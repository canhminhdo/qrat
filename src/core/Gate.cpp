//
// Created by CanhDo on 2024/11/20.
//

#include "core/Gate.hpp"

#include <iostream>
#include <map>
#include <set>

static const char *gateNames[] = {
    "X",
    "Y",
    "Z",
    "H",
    "CX",
};

static const std::map<int, std::set<GateType> > gateArgs = {
    {1, {GateType::X, GateType::Y, GateType::Z, GateType::H}},
    {2, {GateType::CX}},
};

Gate::Gate(GateType type) : type{type} {
    for (auto &pair: gateArgs) {
        if (pair.second.find(type) != pair.second.end()) {
            argsNr = pair.first;
            return;
        }
    }
    argsNr = 0;
    std::cerr << "Invalid number of arguments for gate " << getName() << std::endl;
}

const char *Gate::getName() {
    return gateNames[static_cast<int>(type)];
}

int Gate::getArgsNr() {
    return argsNr;
}
