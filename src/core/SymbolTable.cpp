//
// Created by CanhDo on 2024/11/19.
//

#include "core/SymbolTable.hpp"
#include <iostream>
#include "core/ConstSymbol.hpp"
#include "core/Token.hpp"
#include "core/VarSymbol.hpp"

SymbolTable::SymbolTable() {
    // initialize the symbol table
}

SymbolTable::~SymbolTable() {
    for (auto &entry: simTab) {
        if (VarSymbol *varPtr = dynamic_cast<VarSymbol *>(entry.second)) {
            delete varPtr;
        } else if (ConstSymbol *constPtr = dynamic_cast<ConstSymbol *>(entry.second)) {
            delete constPtr;
        }
    }
    simTab.clear();
}

void SymbolTable::addVarSymbol(int name, Type type, Node *value) {
    if (simTab.find(name) != simTab.end()) {
        std::cerr << "Error: " << Token::name(name) << " variable already exists." << std::endl;
        exit(1);
    }
    simTab[name] = new VarSymbol(name, type, value);
}

void SymbolTable::addConstSymbol(int name, Type type, Node *value, Node *conditions) {
    if (simTab.find(name) != simTab.end()) {
        std::cerr << "Error: " << Token::name(name) << " already exists: " << std::endl;
        exit(1);
    }
    simTab[name] = new ConstSymbol(name, type, value, conditions);
}

Symbol *SymbolTable::lookup(int name) {
    if (simTab.find(name) == simTab.end()) {
        return nullptr;
    }
    return simTab[name];
}

void SymbolTable::dump() const {
    for (auto &entry: simTab) {
        entry.second->dump();
    }
}
