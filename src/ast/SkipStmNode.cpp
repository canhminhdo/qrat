//
// Created by CanhDo on 2024/11/20.
//

#include <iostream>
#include "ast/SkipStmNode.hpp"

void SkipStmNode::dump(bool recursive) {
    std::cout << getName() << std::endl;
    if (recursive && next)
        next->dump();
}
