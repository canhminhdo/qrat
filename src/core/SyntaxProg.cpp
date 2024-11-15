//
// Created by CanhDo on 2024/11/15.
//

#include "core/SyntaxProg.hpp"
#include <iostream>

SyntaxProg::SyntaxProg(Token prog) {
    name = prog.code();
}

SyntaxProg::~SyntaxProg() {
    for (int i = 0; varDecl.size(); i++) {
        delete varDecl[i];
    }
}

void SyntaxProg::addVarDecl(TokenList *variables, Type type) {
    for (int i = 0; i < variables->size(); i++) {
        varDecl.push_back(new VarNode(variables->at(i), type));
    }
}
void SyntaxProg::addConstDecl(TokenList *constants, Type type) {
    for (int i = 0; i < constants->size(); i++) {
        constDecl.push_back(new ConstNode(constants->at(i), type));
    }
}

int SyntaxProg::getName() const {
    return name;
}

void SyntaxProg::dump() const {
    std::cout << "Prog: " << Token::name(name) << std::endl;
    std::cout << "Variable Declaration: " << std::endl;
    for (auto varNode  : varDecl) {
        varNode->dump();
    }
    std::cout << "Constant Declaration: " << std::endl;
    for (auto constNode : constDecl) {
        constNode->dump();
    }
}