//
// Created by CanhDo on 2024/11/19.
//

#ifndef QUBITNODE_HPP
#define QUBITNODE_HPP

#include "ExpNode.hpp"
#include "KetExpNode.hpp"

class QubitExpNode : public ExpNode {
public:
    QubitExpNode(ExpNode *exp, KetExpNode *ket);

    void eval() override {
    }

    void dump() override {
    }

private:
    ExpNode *exp;
    KetExpNode *ket;
};

#endif //QUBITNODE_HPP
