//
// Created by CanhDo on 2024/11/20.
//

#include "ast/SkipStmNode.hpp"
#include <iostream>

void SkipStmNode::dump(bool recursive) {
    std::cout << getName() << std::endl;
    if (recursive && next)
        next->dump();
}

void SkipStmNode::info() {
    std::cout << getName() << ";";
}
