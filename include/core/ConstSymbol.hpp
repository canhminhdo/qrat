//
// Created by CanhDo on 2024/11/19.
//

#ifndef CONSTSYMBOL_HPP
#define CONSTSYMBOL_HPP

#include "ast/Node.hpp"
#include "core/Symbol.hpp"
#include "core/type.hpp"

class ConstSymbol : public Symbol {
public:
    ConstSymbol(int name, Type type, Node *value = nullptr, Node *conditions = nullptr);

    ~ConstSymbol() = default;

    int getName() const override;

    Type getType() const;

    Node *getValue() const;

    Node *getConditions() const;

    void dump() const override;

private:
    int name;
    Type type;
    Node *value;
    Node *conditions;
};

#endif//CONSTSYMBOL_HPP
