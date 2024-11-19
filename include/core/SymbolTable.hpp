//
// Created by CanhDo on 2024/11/19.
//

#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include "Symbol.hpp"
#include "ast/Node.hpp"
#include "core/type.hpp"
#include <unordered_map>

class SymbolTable {
public:
    SymbolTable();
    ~SymbolTable();
    void addVarSymbol(int name, Type type, Node *value = nullptr);
    void addConstSymbol(int name, Type type, Node *value = nullptr, Node *conditions = nullptr);
    Symbol *lookup(int name);
    void dump() const;

private:
    std::pmr::unordered_map<int, Symbol *> simTab;
};

#endif//SYMBOLTABLE_HPP
