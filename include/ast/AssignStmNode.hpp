//
// Created by CanhDo on 2024/11/19.
//

#ifndef ASSIGNSTMNODE_HPP
#define ASSIGNSTMNODE_HPP

#include "ExpNode.hpp"
#include "StmNode.hpp"
#include "core/VarSymbol.hpp"

class AssignStmNode : public StmNode {
public:
    void eval() override {};

    void dump() override {};

private:
    VarSymbol *var;
    ExpNode *exp;
};

#endif // ASSIGNSTMNODE_HPP
