//
// Created by CanhDo on 2024/11/19.
//

#include "ast/NumExpNode.hpp"

#include <cstdlib>
#include <iostream>

NumExpNode::NumExpNode(NumType numType, const char *valStr) : type(numType) {
    if (type == NumType::INT) {
        v.valInt = atoi(valStr);
    } else if (type == NumType::REAL) {
        v.valFloat = atof(valStr);
    } else {
        std::cerr << "Invalid NumType" << std::endl;
    }
}
