//
// Created by CanhDo on 2024/11/20.
//

#include "ast/CondExpNode.hpp"

#include <iostream>

CondExpNode::CondExpNode(ExpNode *left, RelOpType type, ExpNode *right) : left{left}, type{type}, right{right} {
}

const char *CondExpNode::getRelOpName(RelOpType type) {
    return relOpNames[static_cast<int>(type)];
}

void CondExpNode::dump() {
    left->dump();
    std::cout << getRelOpName(type) << std::endl;
    right->dump();
}
