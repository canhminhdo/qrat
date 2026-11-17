//
// Created by CanhDo on 2024/11/15.
//


#include "core/SyntaxProg.hpp"
#include "core/ConstSymbol.hpp"
#include "core/VarSymbol.hpp"
#include <iostream>

SyntaxProg::SyntaxProg(Token prog) {
    name = prog.code();
    nqubits = 0;
    stmSeq = nullptr;
}

SyntaxProg::~SyntaxProg() {
    delete stmSeq;
}

void SyntaxProg::addVarDecl(TokenList *variables, Type type) {
    for (auto variable: *variables) {
        symTab.addVarSymbol(variable.code(), type);
    }
    nqubits += variables->size();
}

void SyntaxProg::addConstDecl(TokenList *constants, Type type) {
    for (auto constant: *constants) {
        symTab.addConstSymbol(constant.code(), type);
    }
}

void SyntaxProg::addInit(Token variable, Node *value) {
    if (auto *varPtr = dynamic_cast<VarSymbol *>(symTab.lookup(variable.code()))) {
        varPtr->setValue(value);
    } else {
        std::cerr << "Error: " << Token::name(variable.code()) << " variable is not declared" << std::endl;
        exit(1);
    }
}

void SyntaxProg::addStmSeq(StmSeq *stmSeq) {
    this->stmSeq = stmSeq;
    // compile the control flow once; the search reads successors from this table
    cfg.build(stmSeq);
}

StmSeq *SyntaxProg::getStmSeq() const {
    return stmSeq;
}

const ControlFlowGraph &SyntaxProg::getCfg() const {
    return cfg;
}

int SyntaxProg::getName() const {
    return name;
}

bool SyntaxProg::hasVarSymbol(const Token &token) {
    if (auto *varPtr = dynamic_cast<VarSymbol *>(symTab.lookup(token.code()))) {
        return true;
    }
    return false;
}

bool SyntaxProg::hasConstSymbol(const Token &token) {
    if (auto *constPtr = dynamic_cast<ConstSymbol *>(symTab.lookup(token.code()))) {
        return true;
    }
    return false;
}

Symbol *SyntaxProg::lookup(const Token &token) {
    return symTab.lookup(token.code());
}

void SyntaxProg::addProp(Token name, ExpNode *value) {
    propTab.addProp(name.code(), value);
}

std::size_t SyntaxProg::getNqubits() const {
    return nqubits;
}

std::vector<VarSymbol *> SyntaxProg::getVars() const {
    return symTab.getVars();
}

const PropTable &SyntaxProg::getPropTab() const {
    return propTab;
}

void SyntaxProg::dump() const {
    std::cout << "Prog: " << Token::name(name) << std::endl;
    std::cout << "nQubits: " << nqubits << std::endl;
    std::cout << "Symbol Table: " << std::endl;
    symTab.dump();
    std::cout << "Statements: " << std::endl;
    if (stmSeq != nullptr)
        stmSeq->dump();
    std::cout << "Control Flow Graph: " << std::endl;
    cfg.dump();
    std::cout << "CachedNodes: " << std::endl;
    cachedNodes.dump();
    std::cout << "Propositions: " << std::endl;
    propTab.dump();
}
