//
// Created by CanhDo on 2024/11/20.
//

#ifndef MEASEXPNODE_HPP
#define MEASEXPNODE_HPP

#include "ExpNode.hpp"
#include "core/Symbol.hpp"

class MeasExpNode : public ExpNode {
public:
    explicit MeasExpNode(Symbol *variable);

    ~MeasExpNode() override = default;

    std::size_t getHash() const override;

    bool isEqual(const Node &other) const override;

    Symbol *getVar() const;

    void eval() override {
    };

    void dump() override;

private:
    Symbol *var;
};
#endif //MEASEXPNODE_HPP
