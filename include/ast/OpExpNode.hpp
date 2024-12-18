//
// Created by CanhDo on 2024/11/19.
//

#ifndef OPEXPNODE_HPP
#define OPEXPNODE_HPP

#include "ast/ExpNode.hpp"

// ODR-usable enum class
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

// // inline initialization with ODR
inline static const char *opExpNames[] = {
    "MINUS",
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "MOD"
};

class OpExpNode : public ExpNode {
public:
    OpExpNode(OpExpType type, ExpNode *left, ExpNode *right);

    ~OpExpNode() override = default;

    const char *getOpName();

    std::size_t getHash() const override;

    bool isEqual(const Node &other) const override;

    void eval() override {
    }

    void dump(bool recursive = false) override;

    ExpNode *getLeft() const;

    ExpNode *getRight() const;

private:
    OpExpType type;
    ExpNode *left;
    ExpNode *right;
};

#endif// OPEXPNODE_HP
