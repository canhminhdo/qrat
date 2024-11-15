//
// Created by CanhDo on 2024/11/15.
//

#include "ast/ConstNode.hpp"
#include <iostream>

ConstNode::ConstNode(Token constant, Type constType) {
    name = constant.code();
    type = constType;
}

void ConstNode::eval() {
}

void ConstNode::dump() const {
    std::cout << "name: " << Token::name(name) << ", type: " << getTypeName() << std::endl;
}

const char *ConstNode::getTypeName() const {
    return typeNames[static_cast<int>(type)];
}
