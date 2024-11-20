//
// Created by CanhDo on 2024/11/19.
//

#include <iostream>
#include "ast/OpExpNode.hpp"

static const char *opExpNames[] = {
    "MINUS",
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "MOD"
};

OpExpNode::OpExpNode(OpExpType type, ExpNode *left, ExpNode *right) : type{type}, left{left}, right{right} {
}

void OpExpNode::dump() {
    std::cout << "OpType: " << getOpName() << std::endl;
    if (left != nullptr) {
        left->dump();
    }
    if (right != nullptr) {
        right->dump();
    }
}

const char *OpExpNode::getOpName() {
    return opExpNames[static_cast<int>(type)];
}