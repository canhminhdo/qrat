//
// Created by CanhDo on 2024/11/19.
//

#include <iostream>
#include "ast/KetExpNode.hpp"

KetExpNode::KetExpNode(KetType type) : type{type} {
    // initialize the ket expression node
}

KetType KetExpNode::getType() const {
    return type;
}

const char *KetExpNode::getName() const {
    return KetNames[static_cast<int>(type)];
}

void KetExpNode::dump() {
    std::cout << getName() << std::endl;
}
