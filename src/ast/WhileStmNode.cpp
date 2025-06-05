//
// Created by CanhDo on 2024/11/20.
//

#include "ast/WhileStmNode.hpp"
#include "ast/EndStmNode.hpp"
#include <iostream>

WhileStmNode::WhileStmNode(ExpNode *cond, StmSeq *body) : cond{cond}, body{body} {
    auto *endWhile = new EndStmNode();
    endWhile->setNext(this);
    body->addStm(endWhile);
}

WhileStmNode::~WhileStmNode() {
    delete body;
}

ExpNode *WhileStmNode::getCond() const {
    return cond;
}

StmSeq *WhileStmNode::getBody() const {
    return body;
}

void WhileStmNode::dump(bool recursive) {
    std::cout << "while " << std::endl;
    cond->dump();
    if (recursive) {
        std::cout << "do " << std::endl;
        body->dump();
        std::cout << "od " << std::endl;
        next->dump();
    } else {
        std::cout << "do ... od" << std::endl;
        next->dump();
    }
}

void WhileStmNode::info() {
    std::cout << "while ";
    cond->info();
    std::cout << " do ";
    body->info();
    std::cout << " od;";
}
