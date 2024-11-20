//
// Created by CanhDo on 2024/11/20.
//

#include <iostream>

#include "ast/GateExpNode.hpp"
#include "core/Token.hpp"

GateExpNode::GateExpNode(Gate *gate, std::vector<VarSymbol *> vars) : gate{gate}, vars{vars} {
}

GateExpNode::~GateExpNode() {
    if (gate != nullptr)
        delete gate;
}

void GateExpNode::dump() {
    std::cout << gate->getName() << "(";
    for (int i = 0; i < vars.size(); i++) {
        std::cout << Token::name(vars[i]->getName()) << ((i < vars.size() - 1) ? ", " : "");
    }
    std::cout << ")" << std::endl;
}
