//
// Created by CanhDo on 2024/11/19.
//

#include "ast/OpExpNode.hpp"
#include <iostream>

const char *opExpNames[] = {
        "MINUS",
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "MOD",
        "AND",
        "OR",
        "NOT",
        "GT",
        "LT",
        "GE",
        "LE",
        "EQ",
        "NE",
        "ASSIGN"
};

OpExpNode::OpExpNode(OpExpType type, ExpNode *left, ExpNode *right) : type{type}, left{left}, right{right} {
}

void OpExpNode::dump() {
    std::cout << "<========== OpExpNode - Start ==========>" << std::endl;
    std::cout << "OpType: " << getOpName() << std::endl;
    std::cout << "Left: " << std::endl;
    if (left != nullptr) {
        left->dump();
    }
    std::cout << "Right: " << std::endl;
    if (right != nullptr) {
        right->dump();
    }
    std::cout << "<========== OpExpNode - End ==========>" << std::endl;
}
const char *OpExpNode::getOpName() {
    return opExpNames[static_cast<int>(type)];
}
