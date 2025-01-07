//
// Created by CanhDo on 2024/11/20.
//

#include "ast/MeasExpNode.hpp"
#include "core/Token.hpp"
#include "utility/HashUtil.hpp"
#include <iostream>

MeasExpNode::MeasExpNode(Symbol *variable) : var{variable} {
}

std::size_t MeasExpNode::getHash() const {
    auto hash = HashUtil::combinedHash(seed, std::hash<Symbol *>{}(var));
    return hash;
}

bool MeasExpNode::isEqual(const Node &other) const {
    const auto *otherMeas = dynamic_cast<const MeasExpNode *>(&other);
    if (otherMeas == nullptr || var != otherMeas->getVar())
        return false;
    return true;
}

Symbol *MeasExpNode::getVar() const {
    return var;
}

void MeasExpNode::dump(bool recursive) {
    std::cout << "M[" << Token::name(var->getName()) << "]" << std::endl;
}

void MeasExpNode::info() {
    std::cout << "M[" << Token::name(var->getName()) << "]";
}
