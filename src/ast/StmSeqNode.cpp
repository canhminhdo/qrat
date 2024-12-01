//
// Created by CanhDo on 2024/11/20.
//

#include <iostream>
#include "ast/StmSeqNode.hpp"

#include "ast/CondStmNode.hpp"

void StmSeqNode::addStm(StmNode *stm) {
    stmSeq.push_back(stm);
}

std::size_t StmSeqNode::getHash() const {
    auto hash = seed;
    for (auto stm : stmSeq) {
        hash = combinedHash(hash, std::hash<StmNode *>{}(stm));
    }
    return hash;
}

bool StmSeqNode::isEqual(const Node &other) const {
    const StmSeqNode *otherStmSeq = dynamic_cast<const StmSeqNode *>(&other);
    if (otherStmSeq == nullptr)
        return false;
    const auto &stmSeq2 = otherStmSeq->getStmSeq();
    if (stmSeq.size() != stmSeq2.size())
        return false;
    for (int i = 0; i < stmSeq.size(); i++) {
        if (stmSeq[i] != stmSeq2[i])
            return false;
    }
    return true;
}

const std::vector<StmNode *> &StmSeqNode::getStmSeq() const {
    return stmSeq;
}

void StmSeqNode::dump() {
    for (int i = 0; i < stmSeq.size(); i++) {
        stmSeq[i]->dump();
    }
}
