//
// Created by CanhDo on 2024/11/19.
//

#include "ast/ConstExpNode.hpp"
#include <iostream>
#include <core/Token.hpp>

ConstExpNode::ConstExpNode(Symbol *symbol) : symbol(dynamic_cast<ConstSymbol *>(symbol)) {
}

void ConstExpNode::dump() {
    std::cout << "Const: " << Token::name(symbol->getName()) << std::endl;
}