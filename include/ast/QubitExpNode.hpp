//
// Created by CanhDo on 2024/11/19.
//

#ifndef QUBITNODE_HPP
#define QUBITNODE_HPP

#include "ast/ExpNode.hpp"

class QubitExpNode : public ExpNode {
public:
    QubitExpNode(ExpNode *exp, ExpNode *ket);

    ~QubitExpNode() override = default;

    bool isEqual(const Node &other) const override;

    std::size_t getHash() const override;

    void eval() override {
    }

    ExpNode *getKet() const;

    ExpNode *getExp() const;

    void dump(bool recursive) override;

    void info() override;

private:
    ExpNode *exp;
    ExpNode *ket;
};

#endif //QUBITNODE_HPP
