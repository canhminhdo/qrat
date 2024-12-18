//
// Created by CanhDo on 2024/12/18.
//

#ifndef ENDWHILESTMNODE_HPP
#define ENDWHILESTMNODE_HPP

#include "StmNode.hpp"

class EndWhileStmNode : public StmNode {
public:
    ~EndWhileStmNode() override = default;

    void eval() override {
    };

    void dump(bool recursive = false) override;
};

#endif //ENDWHILESTMNODE_HPP
