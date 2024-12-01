//
// Created by CanhDo on 2024/11/19.
//

#include "ast/NumExpNode.hpp"

#include <cstdlib>
#include <iostream>

NumExpNode::NumExpNode(NumType numType, const char *valStr) : type(numType) {
    if (type == NumType::INT) {
        valInt = atoi(valStr);
    } else if (type == NumType::REAL) {
        valFloat = atof(valStr);
    } else {
        std::cerr << "Invalid NumType" << std::endl;
    }
}

std::size_t NumExpNode::getHash() const {
    auto hash = combinedHash(seed, std::hash<int>{}(static_cast<int>(type)));
    switch (type) {
        case NumType::INT:
            return combinedHash(hash, std::hash<int>{}(valInt));
        case NumType::REAL:
            return combinedHash(hash, std::hash<float>{}(valFloat));
        default:
            throw std::invalid_argument("Invalid NumType");
    }
}

bool NumExpNode::isEqual(const Node &other) const {
    const NumExpNode *otherNum = dynamic_cast<const NumExpNode *>(&other);
    if (!otherNum) {
        return false;
    }
    if (type != otherNum->type) {
        return false;
    }
    switch (type) {
        case NumType::INT:
            return valInt == otherNum->valInt;
        case NumType::REAL:
            return std::fabs(valFloat - otherNum->valFloat) < 1e-6;
        default:
            throw std::invalid_argument("Invalid NumType");
    }
}

void NumExpNode::dump() {
    if (type == NumType::INT) {
        std::cout << valInt << std::endl;
    } else if (type == NumType::REAL) {
        std::cout << valFloat << std::endl;
    } else {
        std::cerr << "Invalid NumType" << std::endl;
    }
}
