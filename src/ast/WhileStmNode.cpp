//
// Created by CanhDo on 2024/11/20.
//

#include "ast/WhileStmNode.hpp"
#include <iostream>

WhileStmNode::WhileStmNode(ExpNode *cond, StmNode *body) : cond(cond), body(body) {
}

void WhileStmNode::dump() {
    std::cout << "While: " << std::endl;
    cond->dump();
    std::cout << "Do: " << std::endl;
    body->dump();
    std::cout << "Od: " << std::endl;
}
