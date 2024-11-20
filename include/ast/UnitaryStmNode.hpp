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
#include "core/Symbol.hpp"

class UnitaryStmNode : public StmNode {
public:
    UnitaryStmNode(SyntaxProg *currentSyntaxProg, TokenList *vars1, Gate *gate, TokenList *vars2);

    virtual ~UnitaryStmNode();

    void eval() override {
    };

    void dump() override;

private:
    std::vector<Symbol *> vars;
    GateExpNode *gateExp;
};

#endif //UNITARYSTMNODE_HPP
