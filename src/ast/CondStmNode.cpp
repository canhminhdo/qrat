//
// Created by CanhDo on 2024/11/20.
//

#include "ast/CondStmNode.hpp"

#include <iostream>

CondStmNode::CondStmNode(ExpNode *cond, StmNode *then_stm, StmNode *else_stm) : cond(cond), thenStm(then_stm),
    elseStm(else_stm) {
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
