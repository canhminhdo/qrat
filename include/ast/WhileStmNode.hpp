//
// Created by CanhDo on 2024/11/20.
//

#ifndef WHILESTMNODE_HPP
#define WHILESTMNODE_HPP

#include "StmSeq.hpp"
#include "ast/ExpNode.hpp"
#include "ast/StmNode.hpp"

class WhileStmNode : public StmNode {
public:
    WhileStmNode(ExpNode *cond, StmSeq *body);

    ~WhileStmNode();

    ExpNode *getCond() const;

    StmSeq *getBody() const;

    void eval() override {
    };

    void dump(bool recursive) override;

    void info() override;

private:
    ExpNode *cond;
    StmSeq *body;
};

#endif //WHILESTMNODE_HPP
