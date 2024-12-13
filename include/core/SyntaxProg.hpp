//
// Created by CanhDo on 2024/11/15.
//

#ifndef SYNTAXPROG_HPP
#define SYNTAXPROG_HPP

#include "ast/CachedNode.hpp"
#include "ast/StmSeqNode.hpp"
#include "core/SymbolTable.hpp"
#include "core/Token.hpp"
#include "core/type.hpp"

class SyntaxProg {
public:
    SyntaxProg() = default;

    SyntaxProg(Token prog);

    ~SyntaxProg();

    void addVarDecl(TokenList *variables, Type type);

    void addConstDecl(TokenList *variables, Type type);

    void addInit(Token variable, Node *value = nullptr);

    void addStmSeq(StmSeqNode *stmSeq);

    int getName() const;

    bool hasVarSymbol(const Token &token);

    bool hasConstSymbol(const Token &token);

    Symbol *lookup(const Token &token);

    Node *makeNode(Node *node);

    Gate *makeGate(Gate *gate);

    std::size_t getNqubits() const;

    std::vector<VarSymbol *> getVars() const;

    // for debugging
    void dump() const;

private:
    int name; // program name
    std::size_t nqubits; // number of qubits
    SymbolTable symTab; // store variables and constants
    StmSeqNode *stmSeq; // store statements (quantum programs)
    CachedNode cachedNodes; // cached nodes for some expressions in ASTs
};
#endif//SYNTAXPROG_HPP
