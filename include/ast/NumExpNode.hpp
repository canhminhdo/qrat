//
// Created by CanhDo on 2024/11/19.
//

#ifndef NUMEXPNODE_HPP
#define NUMEXPNODE_HPP
#include "ExpNode.hpp"

enum class NumType {
    INT,
    REAL
    // todo: need to support Cpx
};

class NumExpNode : public ExpNode {
public:
    NumExpNode(NumType type, const char *valStr);

    void eval() override {
    }

    void dump() override {
    }

private:
    union Value {
        int valInt;
        float valFloat;
    };

    NumType type;
    Value v;
};

#endif //NUMEXPNODE_HPP
