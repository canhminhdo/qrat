//
// Created by CanhDo on 2024/11/20.
//

#ifndef CONDSTMNODE_HPP
#define CONDSTMNODE_HPP

#include "ExpNode.hpp"
#include "StmNode.hpp"

class CondStmNode : public StmNode {
public:

    CondStmNode() = default;

    CondStmNode(ExpNode *cond, StmNode *then_stm, StmNode *else_stm);

    void eval() override {};

    void dump() override;

private:
    ExpNode *cond;
    StmNode *thenStm;
    StmNode *elseStm;
};

#endif //CONDSTMNODE_HPP
