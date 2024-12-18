//
// Created by CanhDo on 2024/11/19.
//

#include <iostream>
#include "ast/QubitExpNode.hpp"

#include "utility/HashUtil.hpp"

QubitExpNode::QubitExpNode(ExpNode *exp, ExpNode *ket) : exp(exp), ket(ket) {
}

std::size_t QubitExpNode::getHash() const {
    auto hash = HashUtil::combinedHash(seed, std::hash<ExpNode *>{}(ket));
    hash = HashUtil::combinedHash(hash, std::hash<ExpNode *>{}(exp));
    return hash;
}

bool QubitExpNode::isEqual(const Node &other) const {
    const QubitExpNode *otherQubit = dynamic_cast<const QubitExpNode *>(&other);
    if (otherQubit == nullptr || ket != otherQubit->getKet() || exp != otherQubit->getExp())
        return false;
    return true;
}

void QubitExpNode::dump(bool recursive) {
    if (exp != nullptr) {
        std::cout << "expr:" << std::endl;
        exp->dump();
    }
    if (ket != nullptr) {
        std::cout << "ket:" << std::endl;
        ket->dump();
    }
}

ExpNode *QubitExpNode::getKet() const {
    return ket;
}

ExpNode *QubitExpNode::getExp() const {
    return exp;
}
