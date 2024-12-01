//
// Created by CanhDo on 2024/11/20.
//

#include "ast/MeasExpNode.hpp"
#include <iostream>
#include "core/Token.hpp"

MeasExpNode::MeasExpNode(Symbol *variable) : var{variable} {
}

std::size_t MeasExpNode::getHash() const {
    auto hash = combinedHash(seed, std::hash<Symbol *>{}(var));
    return hash;
}

bool MeasExpNode::isEqual(const Node &other) const {
    const auto *otherMeas = dynamic_cast<const MeasExpNode *>(&other);
    if (otherMeas == nullptr || var != otherMeas->getVar())
        return false;
    return true;
}

Symbol * MeasExpNode::getVar() const {
    return var;
}

void MeasExpNode::dump() {
    std::cout << "M[" << Token::name(var->getName()) << "]" << std::endl;
}
