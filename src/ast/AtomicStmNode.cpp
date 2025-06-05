//
// Created by CanhDo on 2025/06/04.
//

#include "ast/AtomicStmNode.hpp"
#include "ast/EndStmNode.hpp"
#include "ast/CondStmNode.hpp"
#include "ast/WhileStmNode.hpp"
#include <iostream>

AtomicStmNode::AtomicStmNode(StmSeq *body) : body{body} {
    body->addStm(new EndStmNode()); // Add an EndStmNode to the end of the body as termination marker
}

AtomicStmNode::~AtomicStmNode() {
    delete body;
}

StmSeq *AtomicStmNode::getBody() const {
    return body;
}

bool AtomicStmNode::isValid() const {
    auto current = body->getHead();
    auto tail = body->getTail();
    while (current != tail) {
        if (dynamic_cast<CondStmNode *>(current) || dynamic_cast<WhileStmNode *>(current)) {
            return false; // Atomic statements should not contain CondStmNode or WhileStmNode
        }
        current = current->getNext();
    }
    return current && tail;
}

void AtomicStmNode::dump(bool recursive) {
    std::cout << "atomic - " << body->size() << std::endl;
    if (recursive) {
        std::cout << "{" << std::endl;
        body->dump();
        std::cout << "}" << std::endl;
        next->dump();
    } else {
        std::cout << "{ ... }" << std::endl;
        next->dump();
    }
}

void AtomicStmNode::info() {
    std::cout << "atomic ";
    std::cout << " { ";
    body->info();
    std::cout << " } ";
}
