//
// Created by CanhDo on 2024/11/20.
//

#include <iostream>

#include "ast/GateExpNode.hpp"
#include "core/Token.hpp"
#include "utility/HashUtil.hpp"

GateExpNode::GateExpNode(Gate *gate, std::vector<Symbol *> vars) : gate{gate}, vars{vars} {
}

std::size_t GateExpNode::getHash() const {
    auto hash = HashUtil::combinedHash(seed, std::hash<Gate *>{}(gate));
    for (auto var: vars) {
        hash = HashUtil::combinedHash(hash, std::hash<Symbol *>{}(var));
    }
    return hash;
}

Gate * GateExpNode::getGate() const {
    return gate;
}

bool GateExpNode::isEqual(const Node &other) const {
    const GateExpNode *otherGate = dynamic_cast<const GateExpNode *>(&other);
    if (otherGate == nullptr || gate != otherGate->gate)
        return false;
    for (int i = 0; i < vars.size(); i++) {
        if (vars[i] != otherGate->vars[i])
            return false;
    }
    return true;
}

void GateExpNode::dump(bool recursive) {
    std::cout << gate->getName() << "[";
    for (int i = 0; i < vars.size(); i++) {
        std::cout << Token::name(vars[i]->getName()) << ((i < vars.size() - 1) ? ", " : "");
    }
    std::cout << "]" << std::endl;
}
