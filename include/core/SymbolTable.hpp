//
// Created by CanhDo on 2024/11/19.
//

#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <unordered_map>
#include <vector>

#include "core/Symbol.hpp"
#include "ast/Node.hpp"
#include "core/type.hpp"
#include "VarSymbol.hpp"

class SymbolTable {
public:
    SymbolTable() = default;

    ~SymbolTable();

    void addVarSymbol(int name, Type type, Node *value = nullptr);

    void addConstSymbol(int name, Type type, Node *value = nullptr, Node *conditions = nullptr);

    Symbol *lookup(int name);

    void dump() const;

    int size() const;

    std::vector<VarSymbol *> getVars() const;

private:
    std::unordered_map<int, Symbol *> simTab;
    std::vector<VarSymbol *> vars;
};

#endif//SYMBOLTABLE_HPP
