//
// Created by CanhDo on 2024/11/20.
//

#include "ast/StmSeq.hpp"
#include "ast/CondStmNode.hpp"
#include "ast/EndStmNode.hpp"
#include <iostream>

StmSeq::StmSeq() : head{nullptr}, tail{nullptr} {
}

StmSeq::~StmSeq() {
    StmNode *current = head;
    while (current != nullptr) {
        if (dynamic_cast<EndStmNode *>(current)) {
            delete current;
            break;
        }
        StmNode *next = current->getNext();
        delete current;
        current = next;
    }
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

StmNode *StmSeq::getHead() const {
    return head;
}

StmNode *StmSeq::getTail() const {
    return tail;
}

void StmSeq::dump(bool recursive) {
    if (head)
        head->dump();
}

int StmSeq::size() const {
    int count = 0;
    auto current = head;
    while (current != nullptr && current != tail) {
        count++;
        current = current->getNext();
    }
    if (dynamic_cast<EndStmNode *>(current) == nullptr)
        count++;
    return count;
}

void StmSeq::info() {
    StmNode *current = head;
    while (current != nullptr && current != tail) {
        current->info();
        current = current->getNext();
    }
    if (dynamic_cast<EndStmNode *>(current) == nullptr)
        current->info();
}
