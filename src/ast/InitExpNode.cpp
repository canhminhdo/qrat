//
// Created by CanhDo on 2024/12/22.
//

#include "ast/InitExpNode.hpp"
#include "core/Token.hpp"
#include "utility/HashUtil.hpp"
#include <iostream>

InitExpNode::InitExpNode(Symbol *var) : var{var} {
}

size_t InitExpNode::getHash() const {
    auto hash = HashUtil::combinedHash(seed, std::hash<const char *>{}(getName()));
    hash = HashUtil::combinedHash(hash, std::hash<Symbol *>{}(var));
    return hash;
}

Symbol * InitExpNode::getVar() const {
    return var;
}

bool InitExpNode::isEqual(const Node &other) const {
    const auto *otherInit = dynamic_cast<const InitExpNode *>(&other);
    if (otherInit == nullptr || var != otherInit->var)
        return false;
    return true;
}

void InitExpNode::dump(bool recursive) {
    std::cout << name << "[" << Token::name(var->getName()) << "]";
}

void InitExpNode::info() {
    std::cout << name << "[" << Token::name(var->getName()) << "]";
}
