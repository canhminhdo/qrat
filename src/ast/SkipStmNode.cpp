//
// Created by CanhDo on 2024/11/20.
//

#include <iostream>
#include "ast/SkipStmNode.hpp"

std::size_t SkipStmNode::getHash() const {
    return combinedHash(seed, std::hash<const char *>{}(getName()));
}

bool SkipStmNode::isEqual(const Node &other) const {
    return dynamic_cast<const SkipStmNode *>(&other) != nullptr;
}

void SkipStmNode::dump() {
    std::cout << getName() << std::endl;
}
