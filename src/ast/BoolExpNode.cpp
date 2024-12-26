//
// Created by CanhDo on 2024/12/23.
//

#include "ast/BoolExpNode.hpp"
#include <functional>
#include "utility/HashUtil.hpp"
#include <iostream>

BoolExpNode::BoolExpNode(BoolType val) : val{val} {
}

std::size_t BoolExpNode::getHash() const {
    return HashUtil::combinedHash(seed, std::hash<int>{}(static_cast<int>(val)));
}

BoolType BoolExpNode::getVal() const {
    return val;
}

bool BoolExpNode::isEqual(const Node &other) const {
    auto *otherBool = dynamic_cast<const BoolExpNode *>(&other);
    if (otherBool == nullptr || val != otherBool->val)
        return false;
    return true;
}

void BoolExpNode::dump(bool recursive) {
    std::cout << boolNames[static_cast<int>(val)] << std::endl;
}
