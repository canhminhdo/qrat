//
// Created by CanhDo on 2024/11/20.
//

#ifndef CONDSTMNODE_HPP
#define CONDSTMNODE_HPP

#include "ExpNode.hpp"
#include "StmNode.hpp"
#include "StmSeq.hpp"

class CondStmNode : public StmNode {
public:
    CondStmNode() = default;

    CondStmNode(ExpNode *cond, StmSeq *then_stm, StmSeq *else_stm);

    ~CondStmNode() override = default;

    ExpNode *getCond() const;

    StmSeq *getThenStm() const;

    StmSeq *getElseStm() const;

    void eval() override {
    };

    void dump(bool recursive = false) override;

private:
    ExpNode *cond;
    StmSeq *thenStm;
    StmSeq *elseStm;
};

#endif //CONDSTMNODE_HPP
