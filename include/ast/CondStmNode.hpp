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

    ~CondStmNode() override = default;

    std::size_t getHash() const override;

    bool isEqual(const Node &other) const override;

    ExpNode *getCond() const;

    StmNode *getThenStm() const;

    StmNode *getElseStm() const;

    void eval() override {
    };

    void dump() override;

private:
    ExpNode *cond;
    StmNode *thenStm;
    StmNode *elseStm;
};

#endif //CONDSTMNODE_HPP
