//
// Created by CanhDo on 2024/11/19.
//

#include "core/SymbolTable.hpp"
#include "core/ConstSymbol.hpp"
#include "core/Token.hpp"
#include "core/VarSymbol.hpp"
#include <iostream>

SymbolTable::~SymbolTable() {
    for (auto &entry: simTab) {
        if (auto *varPtr = dynamic_cast<VarSymbol *>(entry.second)) {
            delete varPtr;
        } else if (auto *constPtr = dynamic_cast<ConstSymbol *>(entry.second)) {
            delete constPtr;
        }
    }
    simTab.clear();
    // variable symbols must be deleted in simTab before the vector is cleared
    vars.clear();
}

void SymbolTable::addVarSymbol(int name, Type type, Node *value) {
    if (simTab.find(name) != simTab.end()) {
        std::cerr << "Error: " << Token::name(name) << " variable already exists" << std::endl;
        exit(1);
    }
    auto *varPtr = new VarSymbol(name, type, value);
    simTab[name] = varPtr;
    vars.push_back(varPtr);
}

void SymbolTable::addConstSymbol(int name, Type type, Node *value, Node *conditions) {
    if (simTab.find(name) != simTab.end()) {
        std::cerr << "Error: " << Token::name(name) << " already exists" << std::endl;
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

int SymbolTable::size() const {
    return vars.size();
}

std::vector<VarSymbol *> SymbolTable::getVars() const {
    return vars;
}
