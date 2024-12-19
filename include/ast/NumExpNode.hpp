//
// Created by CanhDo on 2024/11/19.
//

#ifndef NUMEXPNODE_HPP
#define NUMEXPNODE_HPP

#include "ast/ExpNode.hpp"

enum class NumType {
    INT,
    REAL
    // todo: need to support Cpx
};

class NumExpNode : public ExpNode {
public:
    NumExpNode(NumType type, const char *valStr);

    ~NumExpNode() override = default;

    std::size_t getHash() const override;

    bool isEqual(const Node &other) const override;

    void eval() override {
    }

    void dump(bool recursive = true) override;

private:
    union {
        int valInt;
        float valFloat;
    };

    NumType type;
};

#endif //NUMEXPNODE_HPP
