//
// Created by CanhDo on 2024/11/19.
//

#ifndef OPEXPNODE_HPP
#define OPEXPNODE_HPP

#include "ExpNode.hpp"

enum class OpExpType {
    MINUS,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    AND,
    OR,
    NOT,
    GT,
    LT,
    GE,
    LE,
    EQ,
    NE,
    ASSIGN
};

extern const char *opExpNames[];

class OpExpNode : public ExpNode {
public:
    OpExpNode(OpExpType type, ExpNode *left, ExpNode *right);

    const char *getOpName();

    void eval() override {
    }

    void dump() override;

private:
    OpExpType type;
    ExpNode *left;
    ExpNode *right;
};

#endif// OPEXPNODE_HP
