//
// Created by CanhDo on 2024/11/20.
//

#include <iostream>
#include "ast/StmSeq.hpp"
#include "ast/CondStmNode.hpp"

StmSeq::StmSeq() : head{nullptr}, tail{nullptr} {
}

void StmSeq::addStm(StmNode *stm) {
    if (head == nullptr) {
        head = stm;
        tail = stm;
    } else {
        tail->setNext(stm);
        tail = stm;
    }
}

StmNode * StmSeq::getHead() const {
    return head;
}

StmNode * StmSeq::getTail() const {
    return tail;
}

void StmSeq::dump(bool recursive) {
    if (head)
        head->dump();
}
