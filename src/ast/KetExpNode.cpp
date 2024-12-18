//
// Created by CanhDo on 2024/11/19.
//

#include <iostream>
#include "ast/KetExpNode.hpp"

#include "utility/HashUtil.hpp"

KetExpNode::KetExpNode(KetType type) : type{type} {
    // initialize the ket expression node
}

KetType KetExpNode::getType() const {
    return type;
}

const char *KetExpNode::getName() const {
    return KetNames[static_cast<int>(type)];
}

std::size_t KetExpNode::getHash() const {
    auto hash = HashUtil::combinedHash(seed, std::hash<int>{}(static_cast<int>(type)));
    return hash;
}

bool KetExpNode::isEqual(const Node &other) const {
    const KetExpNode *otherKet = dynamic_cast<const KetExpNode *>(&other);
    if (otherKet == nullptr || type != otherKet->type)
        return false;
    return true;
}

void KetExpNode::dump(bool recursive) {
    std::cout << getName() << std::endl;
}
