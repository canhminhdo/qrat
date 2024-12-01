//
// Created by CanhDo on 2024/11/20.
//

#include <iostream>

#include "ast/GateExpNode.hpp"
#include "core/Token.hpp"

GateExpNode::GateExpNode(Gate *gate, std::vector<Symbol *> vars) : gate{gate}, vars{vars} {
}

GateExpNode::~GateExpNode() {
    if (gate != nullptr)
        delete gate;
}

std::size_t GateExpNode::getHash() const {
    auto hash = combinedHash(seed, std::hash<Gate *>{}(gate));
    for (auto var: vars) {
        hash = combinedHash(hash, std::hash<Symbol *>{}(var));
    }
    return hash;
}

bool GateExpNode::isEqual(const Node &other) const {
    const GateExpNode *otherGate = dynamic_cast<const GateExpNode *>(&other);
    if (otherGate == nullptr || gate->getName() != otherGate->gate->getName() || vars.size() != otherGate->vars.size())
        return false;
    for (int i = 0; i < vars.size(); i++) {
        if (vars[i] != otherGate->vars[i])
            return false;
    }
    return true;
}

void GateExpNode::dump() {
    std::cout << gate->getName() << "[";
    for (int i = 0; i < vars.size(); i++) {
        std::cout << Token::name(vars[i]->getName()) << ((i < vars.size() - 1) ? ", " : "");
    }
    std::cout << "]" << std::endl;
}
