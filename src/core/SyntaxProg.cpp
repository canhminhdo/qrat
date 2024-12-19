//
// Created by CanhDo on 2024/11/15.
//


#include "core/SyntaxProg.hpp"
#include "ast/EndStmNode.hpp"
#include "core/ConstSymbol.hpp"
#include "core/VarSymbol.hpp"
#include <iostream>

#include "ast/CondStmNode.hpp"

SyntaxProg::SyntaxProg(Token prog) {
    name = prog.code();
    nqubits = 0;
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
    nqubits += variables->size();
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

void SyntaxProg::addStmSeq(StmSeq *stmSeq) {
    this->stmSeq = stmSeq;
    // add EndStmNode to the end of stmSeq recursively
    this->stmSeq->addStm(new EndStmNode());
    addEndStm(stmSeq);
}

void SyntaxProg::addEndStm(StmSeq *stmSeq) {
    auto head = stmSeq->getHead();
    while (head != nullptr) {
        if (dynamic_cast<EndStmNode *>(head)) {
            break;
        }
        StmNode* next = head->getNext();
        auto *condStm = dynamic_cast<CondStmNode *>(head);
        if (condStm != nullptr) {
            auto *endStm = new EndStmNode(next);
            condStm->getThenStm()->addStm(endStm);
            condStm->getElseStm()->addStm(endStm);
            addEndStm(condStm->getThenStm());
            addEndStm(condStm->getElseStm());
        }
        head = next;
    }
}

StmSeq * SyntaxProg::getStmSeq() const {
    return stmSeq;
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

ExpNode *SyntaxProg::makeNode(ExpNode *node) {
    return cachedNodes.makeNode(node);
}

Gate *SyntaxProg::makeGate(Gate *gate) {
    return cachedNodes.makeGate(gate);
}

std::size_t SyntaxProg::getNqubits() const {
    return nqubits;
}

std::vector<VarSymbol *> SyntaxProg::getVars() const {
    return symTab.getVars();
}

void SyntaxProg::dump() const {
    std::cout << "Prog: " << Token::name(name) << std::endl;
    std::cout << "nQubits: " << nqubits << std::endl;
    std::cout << "Symbol Table: " << std::endl;
    symTab.dump();
    std::cout << "Statements: " << std::endl;
    stmSeq->dump();
    std::cout << "CachedNodes: " << std::endl;
    cachedNodes.dump();
}
