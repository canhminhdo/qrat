//
// Created by CanhDo on 2024/11/19.
//

#include <iostream>

#include "ast/ConstExpNode.hpp"
#include "core/Token.hpp"

ConstExpNode::ConstExpNode(Symbol *symbol) : symbol(symbol) {
}

void ConstExpNode::dump() {
    std::cout << Token::name(symbol->getName()) << std::endl;
}
