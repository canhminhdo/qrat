//
// Created by CanhDo on 2024/11/20.
//

#include "ast/SkipStmNode.hpp"
#include <functional>
#include <iostream>
#include <string_view>

void SkipStmNode::dump(bool recursive) {
    std::cout << getName() << std::endl;
}

std::size_t SkipStmNode::getHash() const {
    return std::hash<std::string_view>{}(name);
}

bool SkipStmNode::isEqual(const Node &other) const {
    return dynamic_cast<const SkipStmNode *>(&other) != nullptr;
}

bool SkipStmNode::isShared() const {
    return true;
}

void SkipStmNode::info() {
    std::cout << getName() << ";";
}
