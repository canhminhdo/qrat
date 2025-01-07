//
// Created by CanhDo on 2024/11/19.
//

#include "ast/OpExpNode.hpp"
#include <iostream>

#include "utility/HashUtil.hpp"

OpExpNode::OpExpNode(OpExpType type, ExpNode *left, ExpNode *right) : type{type}, left{left}, right{right} {
}

ExpNode *OpExpNode::getLeft() const {
    return left;
}

ExpNode *OpExpNode::getRight() const {
    return right;
}

OpExpType OpExpNode::getType() const {
    return type;
}

const char *OpExpNode::getOpName() {
    return opExpNames[static_cast<int>(type)];
}

std::size_t OpExpNode::getHash() const {
    auto hash = HashUtil::combinedHash(seed, std::hash<int>{}(static_cast<int>(type)));
    hash = HashUtil::combinedHash(hash, std::hash<ExpNode *>{}(left));
    hash = HashUtil::combinedHash(hash, std::hash<ExpNode *>{}(right));
    return hash;
}

bool OpExpNode::isEqual(const Node &other) const {
    const OpExpNode *otherOp = dynamic_cast<const OpExpNode *>(&other);
    if (otherOp == nullptr || type != otherOp->type || left != otherOp->left || right != otherOp->right)
        return false;
    return true;
}

void OpExpNode::dump(bool recursive) {
    std::cout << "OpType: " << getOpName() << std::endl;
    if (left != nullptr) {
        left->dump();
    }
    if (right != nullptr) {
        right->dump();
    }
}

void OpExpNode::info() {
    if (left != nullptr) {
        left->info();
    }
    std::cout << " " << getOpName() << " ";
    if (right != nullptr) {
        right->info();
    }
}