//
// Created by CanhDo on 2024/11/20.
//

#ifndef CONDEXPNODE_HPP
#define CONDEXPNODE_HPP

#include "ast/ExpNode.hpp"

// ODR-usable enum class
enum class RelOpType {
    EQ,
    // todo: support the remaining relational operator
    NE,
    GT,
    LT,
    GE,
    LE,
    AND,
    OR,
    NOT,
};

// inline initialization with ODR
inline static const char *relOpNames[] = {
    "EQ",
    "NE",
    "GT",
    "LT",
    "GE",
    "LE",
    "AND",
    "OR",
    "NOT"
};

class CondExpNode : public ExpNode {
public:
    CondExpNode(ExpNode *left, RelOpType type, ExpNode *right);

    void eval() override {
    };

    const char *getRelOpName(RelOpType type);

    void dump() override;

private:
    ExpNode *left;
    RelOpType type;
    ExpNode *right;
};
#endif //CONDEXPNODE_HPP
