//
// Created by CanhDo on 2024/11/20.
//

#include "ast/CondStmNode.hpp"

#include <iostream>

CondStmNode::CondStmNode(ExpNode *cond, StmNode *then_stm, StmNode *else_stm) : cond(cond), thenStm(then_stm),
    elseStm(else_stm) {
}

std::size_t CondStmNode::getHash() const {
    auto hash = combinedHash(seed, std::hash<ExpNode *>{}(cond));
    hash = combinedHash(hash, std::hash<StmNode *>{}(thenStm));
    hash = combinedHash(hash, std::hash<StmNode *>{}(elseStm));
    return hash;
}

bool CondStmNode::isEqual(const Node &other) const {
    const auto *otherCondStm = dynamic_cast<const CondStmNode *>(&other);
    if (otherCondStm == nullptr || cond != otherCondStm->getCond() || thenStm != otherCondStm->getThenStm() ||
        elseStm != otherCondStm->getElseStm())
        return false;
    return true;
}

ExpNode *CondStmNode::getCond() const {
    return cond;
}

StmNode *CondStmNode::getThenStm() const {
    return thenStm;
}

StmNode *CondStmNode::getElseStm() const {
    return elseStm;
}

void CondStmNode::dump() {
    std::cout << "If: " << std::endl;
    cond->dump();
    std::cout << "Then: " << std::endl;
    thenStm->dump();
    std::cout << "Else: " << std::endl;
    elseStm->dump();
    std::cout << "Fi: " << std::endl;
}
