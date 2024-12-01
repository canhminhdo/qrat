//
// Created by CanhDo on 2024/11/19.
//

#include <iostream>
#include "ast/OpExpNode.hpp"

OpExpNode::OpExpNode(OpExpType type, ExpNode *left, ExpNode *right) : type{type}, left{left}, right{right} {
}

void OpExpNode::dump() {
    std::cout << "OpType: " << getOpName() << std::endl;
    if (left != nullptr) {
        left->dump();
    }
    if (right != nullptr) {
        right->dump();
    }
}

ExpNode * OpExpNode::getLeft() const {
    return left;
}

ExpNode * OpExpNode::getRight() const {
    return right;
}

const char *OpExpNode::getOpName() {
    return opExpNames[static_cast<int>(type)];
}

std::size_t OpExpNode::getHash() const {
    auto hash = combinedHash(seed, std::hash<int>{}(static_cast<int>(type)));
    hash = combinedHash(hash, std::hash<ExpNode *>{}(left));
    hash = combinedHash(hash, std::hash<ExpNode *>{}(right));
    return hash;
}

bool OpExpNode::isEqual(const Node &other) const {
    const OpExpNode *otherOp = dynamic_cast<const OpExpNode *>(&other);
    if (otherOp == nullptr || type != otherOp->type || left != otherOp->left || right != otherOp->right)
        return false;
    return true;
}
