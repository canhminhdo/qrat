//
// Created by CanhDo on 2024/11/20.
//

#include "ast/CondStmNode.hpp"

#include <iostream>

CondStmNode::CondStmNode(ExpNode *cond, StmSeq *then_stm, StmSeq *else_stm) : cond{cond}, thenStm{then_stm},
    elseStm{else_stm} {
}

ExpNode *CondStmNode::getCond() const {
    return cond;
}

StmSeq *CondStmNode::getThenStm() const {
    return thenStm;
}

StmSeq *CondStmNode::getElseStm() const {
    return elseStm;
}

void CondStmNode::dump(bool recursive) {
    std::cout << "If: " << std::endl;
    cond->dump();
    std::cout << "Then: " << std::endl;
    thenStm->dump();
    std::cout << "Else: " << std::endl;
    elseStm->dump();
    std::cout << "Fi: " << std::endl;
    if (recursive && next)
        next->dump();
}
