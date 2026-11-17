//
// Created by CanhDo on 2025/06/04.
//

#include "ast/AtomicStmNode.hpp"
#include "ast/CondStmNode.hpp"
#include "ast/WhileStmNode.hpp"
#include <iostream>

AtomicStmNode::AtomicStmNode(StmSeq *body) : body{body} {
}

AtomicStmNode::~AtomicStmNode() {
    delete body;
}

StmSeq *AtomicStmNode::getBody() const {
    return body;
}

bool AtomicStmNode::isValid() const {
    for (auto *stm: body->getStms()) {
        if (dynamic_cast<CondStmNode *>(stm) || dynamic_cast<WhileStmNode *>(stm)) {
            return false;
        }
    }
    return !body->empty();
}

void AtomicStmNode::dump(bool recursive) {
    std::cout << "atomic - " << body->size() << std::endl;
    if (recursive) {
        std::cout << "{" << std::endl;
        body->dump();
        std::cout << "}" << std::endl;
    } else {
        std::cout << "{ ... }" << std::endl;
    }
}

void AtomicStmNode::info() {
    std::cout << "atomic ";
    std::cout << " { ";
    body->info();
    std::cout << " } ";
}
