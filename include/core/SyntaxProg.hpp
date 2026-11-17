//
// Created by CanhDo on 2024/11/15.
//

#ifndef SYNTAXPROG_HPP
#define SYNTAXPROG_HPP

#include "PropTable.hpp"
#include "ast/CachedNode.hpp"
#include "ast/ExpNode.hpp"
#include "ast/StmSeq.hpp"
#include "core/ControlFlowGraph.hpp"
#include "core/SymbolTable.hpp"
#include "core/Token.hpp"
#include "core/type.hpp"

class SyntaxProg {
public:
    SyntaxProg() = default;

    SyntaxProg(Token prog);

    ~SyntaxProg();

    void addVarDecl(TokenList *variables, Type type);

    void addConstDecl(TokenList *constants, Type type);

    void addInit(Token variable, Node *value = nullptr);

    void addStmSeq(StmSeq *stmSeq);

    StmSeq *getStmSeq() const;

    const ControlFlowGraph &getCfg() const;

    int getName() const;

    bool hasVarSymbol(const Token &token);

    bool hasConstSymbol(const Token &token);

    Symbol *lookup(const Token &token);

    // Caching for expressions and for statements that can be shared (gates and skip)
    template <class T>
    T *makeNode(T *node) {
        return static_cast<T *>(cachedNodes.makeNode(node));
    }

    void addProp(Token name, ExpNode *value);

    std::size_t getNqubits() const;

    std::vector<VarSymbol *> getVars() const;

    const PropTable &getPropTab() const;

    // for debugging
    void dump() const;

private:
    int name; // program name
    std::size_t nqubits; // number of qubits
    SymbolTable symTab; // store variables and constants
    StmSeq *stmSeq; // store statements (quantum programs)
    ControlFlowGraph cfg; // control flow of the statements, built once in addStmSeq()
    CachedNode cachedNodes; // owns the shared nodes of the ASTs: expressions, gate and skip statements
    PropTable propTab; // store propositions
};
#endif//SYNTAXPROG_HPP
