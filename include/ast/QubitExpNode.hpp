//
// Created by CanhDo on 2024/11/19.
//

#ifndef QUBITNODE_HPP
#define QUBITNODE_HPP

#include "ast/ExpNode.hpp"

class QubitExpNode : public ExpNode {
public:
    QubitExpNode(ExpNode *exp, ExpNode *ket);

    void eval() override {
    }

    void dump() override;

private:
    ExpNode *exp;
    ExpNode *ket;
};

#endif //QUBITNODE_HPP
