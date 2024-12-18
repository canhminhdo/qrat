//
// Created by CanhDo on 2024/11/20.
//

#include "ast/WhileStmNode.hpp"
#include "ast/EndStmNode.hpp"
#include <iostream>


WhileStmNode::WhileStmNode(ExpNode *cond, StmSeq *body) : cond{cond}, body{body} {
    EndStmNode *endWhile = new EndStmNode();
    endWhile->setNext(this);
    body->addStm(endWhile);
}

ExpNode *WhileStmNode::getCond() const {
    return cond;
}

StmSeq *WhileStmNode::getBody() const {
    return body;
}

void WhileStmNode::dump(bool recursive) {
    std::cout << "While: " << std::endl;
    cond->dump();
    std::cout << "Do: " << std::endl;
    body->dump();
    std::cout << "Od: " << std::endl;
    if (recursive && next)
        next->dump();
}
