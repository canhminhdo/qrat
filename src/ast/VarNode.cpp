//
// Created by CanhDo on 2024/11/15.
//

#include "ast/VarNode.hpp"
#include <iostream>

VarNode::VarNode(Token variable, Type varType) {
    name = variable.code();
    type = varType;
}

void VarNode::eval() {
}

int VarNode::getName() const {
    return name;
}

void VarNode::dump() const {
    std::cout << "name: " << Token::name(name) << ", type: " << getTypeName() << std::endl;
}

const char *VarNode::getTypeName() const {
    return typeNames[static_cast<int>(type)];
}