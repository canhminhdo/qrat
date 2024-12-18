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
    if (otherCond == nullptr || left != otherCond->get_left() || type != otherCond->get_type() || right != otherCond->
        get_right())
        return false;
    return true;
}

ExpNode *CondExpNode::get_left() const {
    return left;
}

RelOpType CondExpNode::get_type() const {
    return type;
}

ExpNode *CondExpNode::get_right() const {
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
