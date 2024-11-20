//
// Created by CanhDo on 2024/11/20.
//

#include "ast/MeasExpNode.hpp"

#include <iostream>

#include "core/Token.hpp"


MeasExpNode::MeasExpNode(Symbol *variable) : var{variable} {
}

void MeasExpNode::dump() {
    std::cout << "M[" << Token::name(var->getName()) << "]" << std::endl;
}
