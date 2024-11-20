//
// Created by CanhDo on 2024/11/20.
//

#ifndef SKIPSTMNODE_HPP
#define SKIPSTMNODE_HPP

#include "ast/StmNode.hpp"

class SkipStmNode : public StmNode {
public:
    void eval() override {
    };

    void dump() override;

    const char *getName();

private:
    const char *name = "skip";
};


inline const char *SkipStmNode::getName() {
    return name;
}

#endif //SKIPSTMNODE_HPP
