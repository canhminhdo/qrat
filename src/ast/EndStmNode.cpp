//
// Created by CanhDo on 2024/12/18.
//

#include "ast/EndStmNode.hpp"
#include <functional>
#include <iostream>

EndStmNode::EndStmNode(StmNode *next) {
    this->next = next;
}

void EndStmNode::dump(bool recursive) {
    std::cout << "End" << std::endl;
}

void EndStmNode::info() {
}
