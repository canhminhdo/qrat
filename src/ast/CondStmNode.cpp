//
// Created by CanhDo on 2024/11/20.
//

#include "ast/CondStmNode.hpp"

#include <iostream>

CondStmNode::CondStmNode(ExpNode *cond, StmSeq *then_stm, StmSeq *else_stm) : cond{cond}, thenStm{then_stm},
                                                                              elseStm{else_stm} {
}

CondStmNode::~CondStmNode() {
    delete thenStm;
    delete elseStm;
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

void CondStmNode::info() {
    std::cout << "if ";
    cond->info();
    std::cout << " then ";
    thenStm->info();
    std::cout << " else ";
    elseStm->info();
    std::cout << " fi;";
}

void CondStmNode::dump(bool recursive) {
    std::cout << "if" << std::endl;
    cond->dump();
    if (recursive) {
        std::cout << "then" << std::endl;
        thenStm->dump();
        std::cout << "else" << std::endl;
        elseStm->dump();
        std::cout << "fi" << std::endl;
        next->dump();
    } else {
        std::cout << "then ... else ... fi" << std::endl;
    }
}
