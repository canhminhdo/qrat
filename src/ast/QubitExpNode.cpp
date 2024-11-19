//
// Created by CanhDo on 2024/11/19.
//

#include "ast/QubitExpNode.hpp"
#include <iostream>

QubitExpNode::QubitExpNode(ExpNode *exp, KetExpNode *ket) : exp(exp), ket(ket) {
}

void QubitExpNode::dump() {
    std::cout << "<========== QubitExpNode - Start ==========>" << std::endl;
    if (exp != nullptr) {
        exp->dump();
    }
    if (ket != nullptr) {
        ket->dump();
    }
    std::cout << "<========== QubitExpNode - End ==========>" << std::endl;
}
