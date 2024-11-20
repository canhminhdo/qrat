//
// Created by CanhDo on 2024/11/20.
//

#ifndef WHILESTMNODE_HPP
#define WHILESTMNODE_HPP

#include "ast/ExpNode.hpp"
#include "ast/StmNode.hpp"

class WhileStmNode : public StmNode {
public:
    WhileStmNode(ExpNode *cond, StmNode *body);

    void eval() override {
    };

    void dump() override;

private:
    ExpNode *cond;
    StmNode *body;
};

#endif //WHILESTMNODE_HPP
