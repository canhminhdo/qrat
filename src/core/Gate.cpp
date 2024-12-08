//
// Created by CanhDo on 2024/11/20.
//

#include "core/Gate.hpp"
#include <iostream>

Gate::Gate(GateType type) : type{type} {
    int count = 0;
    for (auto &gateArg : gateArgs) {
        if (gateArg.find(type) != gateArg.end()) {
            argsNr = count;
            return;
        }
        count ++;
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

GateType Gate::getType() {
    return type;
}

void Gate::dump() {
    std::cout << getName() << std::endl;
}
