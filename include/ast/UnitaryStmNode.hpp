//
// Created by CanhDo on 2024/11/20.
//

#ifndef UNITARYSTMNODE_HPP
#define UNITARYSTMNODE_HPP

#include "ast/GateExpNode.hpp"
#include "ast/StmNode.hpp"
#include "core/Gate.hpp"
#include "core/SyntaxProg.hpp"
#include "core/Token.hpp"
#include "core/VarSymbol.hpp"

class UnitaryStmNode : public StmNode {
public:
    UnitaryStmNode(SyntaxProg *currentSyntaxProg, TokenList *vars1, Gate *gate, TokenList *vars2);

    virtual ~UnitaryStmNode();

    void eval() override {
    };

    void dump() override;

private:
    std::vector<VarSymbol *> vars;
    GateExpNode *gateExp;
};

#endif //UNITARYSTMNODE_HPP
