//
// Created by CanhDo on 2024/11/19.
//

#include <iostream>

#include "ast/ConstExpNode.hpp"
#include "core/Token.hpp"

ConstExpNode::ConstExpNode(Symbol *symbol) : symbol(symbol) {
}

std::size_t ConstExpNode::getHash() const {
    auto hash = combinedHash(seed, std::hash<Symbol *>{}(symbol));
    return hash;
}

bool ConstExpNode::isEqual(const Node &other) const {
    const ConstExpNode *otherConst = dynamic_cast<const ConstExpNode *>(&other);
    if (otherConst == nullptr || symbol != otherConst->getSymbol())
        return false;
    return true;
}

Symbol * ConstExpNode::getSymbol() const {
    return symbol;
}

void ConstExpNode::dump() {
    std::cout << Token::name(symbol->getName()) << std::endl;
}
