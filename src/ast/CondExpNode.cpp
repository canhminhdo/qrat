//
// Created by CanhDo on 2024/11/20.
//

#include "ast/CondExpNode.hpp"

#include <iostream>

#include "utility/HashUtil.hpp"

CondExpNode::CondExpNode(ExpNode *left, RelOpType type, ExpNode *right) : left{left}, type{type}, right{right} {
}

std::size_t CondExpNode::getHash() const {
    auto hash = HashUtil::combinedHash(seed, std::hash<RelOpType>{}(type));
    hash = HashUtil::combinedHash(hash, std::hash<ExpNode *>{}(left));
    hash = HashUtil::combinedHash(hash, std::hash<ExpNode *>{}(right));
    return hash;
}

bool CondExpNode::isEqual(const Node &other) const {
    const auto *otherCond = dynamic_cast<const CondExpNode *>(&other);
    if (otherCond == nullptr || left != otherCond->getLeft() || type != otherCond->getRelOpType() || right != otherCond->getRight())
        return false;
    return true;
}

ExpNode *CondExpNode::getLeft() const {
    return left;
}

RelOpType CondExpNode::getRelOpType() const {
    return type;
}

ExpNode *CondExpNode::getRight() const {
    return right;
}

const char *CondExpNode::getRelOpName(RelOpType type) {
    return relOpNames[static_cast<int>(type)];
}

void CondExpNode::dump(bool recursive) {
    left->dump();
    std::cout << getRelOpName(type) << std::endl;
    right->dump();
}

void CondExpNode::info() {
    left->info();
    std::cout << " " << getRelOpName(type) << " ";
    right->info();
}
