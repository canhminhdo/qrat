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

    ~WhileStmNode() override = default;

    std::size_t getHash() const override;

    bool isEqual(const Node &other) const override;

    ExpNode *getCond() const;

    StmNode *getBody() const;

    void eval() override {
    };

    void dump() override;

private:
    ExpNode *cond;
    StmNode *body;
};

#endif //WHILESTMNODE_HPP
