//
// Created by CanhDo on 2024/12/23.
//

#ifndef BOOLEXPNODE_HPP
#define BOOLEXPNODE_HPP
#include "ExpNode.hpp"

enum class BoolType {
    TRUE,
    FALSE
};

inline static const char *boolNames[] = {
    "true",
    "false",
};

class BoolExpNode : public ExpNode {
public:
    explicit BoolExpNode(BoolType val);

    std::size_t getHash() const override;

    BoolType getVal() const;

    bool isEqual(const Node &other) const override;

    void eval() override {
    }

    void dump(bool recursive = true) override;

    void info() override;

private:
    BoolType val;
};

#endif //BOOLEXPNODE_HPP
