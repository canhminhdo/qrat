//
// Created by CanhDo on 2024/11/20.
//

#include <iostream>
#include "ast/StmSeqNode.hpp"

void StmSeqNode::addStm(StmNode *stm) {
    stmSeq.push_back(stm);
}

void StmSeqNode::dump() {
    for (int i = 0; i < stmSeq.size(); i++) {
        stmSeq[i]->dump();
    }
}
