//
// Created by CanhDo on 2024/12/18.
//

#ifndef ENDSTMNODE_HPP
#define ENDSTMNODE_HPP

#include "StmNode.hpp"

class EndStmNode : public StmNode {
public:
    EndStmNode() = default;
    ~EndStmNode() override = default;

    void eval() override {
    };

    void dump(bool recursive = false) override;
};

#endif //ENDSTMNODE_HPP
