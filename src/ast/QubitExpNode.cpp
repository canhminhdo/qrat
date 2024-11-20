//
// Created by CanhDo on 2024/11/19.
//

#include <iostream>
#include "ast/QubitExpNode.hpp"

QubitExpNode::QubitExpNode(ExpNode *exp, ExpNode *ket) : exp(exp), ket(ket) {
}

void QubitExpNode::dump() {
    if (exp != nullptr) {
        std::cout << "expr:" << std::endl;
        exp->dump();
    }
    if (ket != nullptr) {
        std::cout << "ket:" << std::endl;
        ket->dump();
    }
}
