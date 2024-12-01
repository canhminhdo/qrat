//
// Created by CanhDo on 2024/11/20.
//

#include "ast/WhileStmNode.hpp"
#include <iostream>

WhileStmNode::WhileStmNode(ExpNode *cond, StmNode *body) : cond(cond), body(body) {
}

std::size_t WhileStmNode::getHash() const {
    auto hash = combinedHash(seed, std::hash<ExpNode *>{}(cond));
    hash = combinedHash(hash, std::hash<StmNode *>{}(body));
    return hash;
}

bool WhileStmNode::isEqual(const Node &other) const {
    const auto *otherWhile = dynamic_cast<const WhileStmNode *>(&other);
    if (otherWhile == nullptr || cond != otherWhile->getCond() || body != otherWhile->getBody())
        return false;
    return true;
}

ExpNode * WhileStmNode::getCond() const {
    return cond;
}

StmNode * WhileStmNode::getBody() const {
    return body;
}

void WhileStmNode::dump() {
    std::cout << "While: " << std::endl;
    cond->dump();
    std::cout << "Do: " << std::endl;
    body->dump();
    std::cout << "Od: " << std::endl;
}
