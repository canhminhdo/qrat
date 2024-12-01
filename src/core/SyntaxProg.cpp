//
// Created by CanhDo on 2024/11/15.
//


#include "core/SyntaxProg.hpp"
#include <iostream>
#include "core/ConstSymbol.hpp"
#include "core/VarSymbol.hpp"

SyntaxProg::SyntaxProg(Token prog) {
    name = prog.code();
}

SyntaxProg::~SyntaxProg() {
    // for (int i = 0; varDecl.size(); i++) {
    //     delete varDecl[i];
    // }
}

void SyntaxProg::addVarDecl(TokenList *variables, Type type) {
    for (int i = 0; i < variables->size(); i++) {
        symTab.addVarSymbol(variables->at(i).code(), type);
    }
}

void SyntaxProg::addConstDecl(TokenList *constants, Type type) {
    for (int i = 0; i < constants->size(); i++) {
        symTab.addConstSymbol(constants->at(i).code(), type);
    }
}

void SyntaxProg::addInit(Token variable, Node *value) {
    if (auto *varPtr = dynamic_cast<VarSymbol *>(symTab.lookup(variable.code()))) {
        varPtr->setValue(value);
    } else {
        std::cerr << "Error: " << Token::name(variable.code()) << " variable is not declared. " << std::endl;
        exit(1);
    }
}

void SyntaxProg::addStmSeq(StmSeqNode *stmSeq) {
    this->stmSeq = stmSeq;
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

Node *SyntaxProg::makeNode(Node *node) {
    return cachedNodes.makeNode(node);
}

void SyntaxProg::dump() const {
    std::cout << "Prog: " << Token::name(name) << std::endl;
    std::cout << "Symbol Table: " << std::endl;
    symTab.dump();
    std::cout << "Statements: " << std::endl;
    stmSeq->dump();
    std::cout << "CachedNodes: " << std::endl;
    cachedNodes.dump();
}
