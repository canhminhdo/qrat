//
// Created by CanhDo on 2024/11/19.
//

#ifndef STMNODE_HPP
#define STMNODE_HPP

#include "ast/Node.hpp"

class StmNode : public Node {
public:
    void setNext(StmNode *next);

    StmNode *getNext();

protected:
    StmNode *next;
};

inline void StmNode::setNext(StmNode *next) {
    this->next = next;
}

inline StmNode *StmNode::getNext() {
    return next;
}

#endif //STMNODE_HPP
