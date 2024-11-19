//
// Created by CanhDo on 2024/11/19.
//

#ifndef VARIABLESYMBOL_HPP
#define VARIABLESYMBOL_HPP

#include "ast/Node.hpp"
#include "core/Symbol.hpp"
#include "core/type.hpp"

class VarSymbol : public Symbol {
public:
    VarSymbol(int name, Type type, Node *value = nullptr);

    ~VarSymbol();

    int getName() const override;

    Type getType() const;

    void setValue(Node *value);

    void dump() const override;

private:
    int name;
    Type type;
    Node *value;
};

#endif//VARIABLESYMBOL_HPP
