//
// Created by CanhDo on 2024/11/15.
//

#ifndef SYNTAXPROG_HPP
#define SYNTAXPROG_HPP

#include "ast/ConstNode.hpp"
#include "ast/VarNode.hpp"
#include "core/Token.hpp"
#include "core/type.hpp"
#include <vector>

class SyntaxProg {
public:
    SyntaxProg() = default;
    SyntaxProg(Token prog);

    ~SyntaxProg();

    void addVarDecl(TokenList *variables, Type type);
    void addConstDecl(TokenList *variables, Type type);
    int getName() const;

    // for debugging
    void dump() const;

private:
    int name;
    std::vector<VarNode *> varDecl;
    std::vector<ConstNode *> constDecl;
};
#endif//SYNTAXPROG_HPP
