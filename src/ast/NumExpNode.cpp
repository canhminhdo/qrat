//
// Created by CanhDo on 2024/11/19.
//

#include "ast/NumExpNode.hpp"

#include <cstdlib>
#include <iostream>

#include "utility/HashUtil.hpp"

NumExpNode::NumExpNode(NumType numType, const char *valStr) : type(numType) {
    if (type == NumType::INT) {
        val.intV = atoi(valStr);
    } else if (type == NumType::REAL) {
        val.floatV = atof(valStr);
    } else {
        throw std::invalid_argument("Unsupported NumType");
    }
}

std::size_t NumExpNode::getHash() const {
    auto hash = HashUtil::combinedHash(seed, std::hash<int>{}(static_cast<int>(type)));
    switch (type) {
        case NumType::INT:
            return HashUtil::combinedHash(hash, std::hash<int>{}(val.intV));
        case NumType::REAL:
            return HashUtil::combinedHash(hash, std::hash<float>{}(val.floatV));
        default:
            throw std::invalid_argument("Unsupported NumType");
    }
}

bool NumExpNode::isEqual(const Node &other) const {
    const auto *otherNum = dynamic_cast<const NumExpNode *>(&other);
    if (!otherNum) {
        return false;
    }
    if (type != otherNum->type) {
        return false;
    }
    switch (type) {
        case NumType::INT:
            return val.intV == otherNum->val.intV;
        case NumType::REAL:
            return std::fabs(val.floatV - otherNum->val.floatV) < 1e-6;
        default:
            throw std::invalid_argument("Unsupported NumType");
    }
}

bool NumExpNode::isInt() {
    return type == NumType::INT;
}

int NumExpNode::getIntVal() const {
    return val.intV;
}

float NumExpNode::getFloatVal() const {
    return val.floatV;
}

bool NumExpNode::isZeroOrOne() const {
    if (type == NumType::INT) {
        return val.intV == 0 || val.intV == 1;
    } else if (type == NumType::REAL) {
        return val.floatV == 0.0 || val.floatV == 1.0;
    } else {
        throw std::invalid_argument("Unsupported NumType");
    }
}

bool NumExpNode::isZero() const {
    if (type == NumType::INT) {
        return val.intV == 0;
    } else if (type == NumType::REAL) {
        return val.floatV == 0.0;
    } else {
        throw std::invalid_argument("Unsupported NumType");
    }
}

bool NumExpNode::isOne() const {
    if (type == NumType::INT) {
        return val.intV == 1;
    } else if (type == NumType::REAL) {
        return val.floatV == 1.0;
    } else {
        throw std::invalid_argument("Unsupported NumType");
    }
}

void NumExpNode::dump(bool recursive) {
    if (type == NumType::INT) {
        std::cout << val.intV << std::endl;
    } else if (type == NumType::REAL) {
        std::cout << val.floatV << std::endl;
    } else {
        throw std::invalid_argument("Unsupported NumType");
    }
}

void NumExpNode::info() {
    if (type == NumType::INT) {
        std::cout << val.intV;
    } else if (type == NumType::REAL) {
        std::cout << val.floatV;
    } else {
        throw std::invalid_argument("Unsupported NumType");
    }
}