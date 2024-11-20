//
// Created by CanhDo on 2024/11/19.
//

#include <iostream>
#include "core/ConstSymbol.hpp"
#include "core/Token.hpp"

ConstSymbol::ConstSymbol(int constName, Type constType, Node *value, Node *conditions) : name{constName},
    type{constType}, value{value}, conditions{conditions} {
    // initialize the constant symbol
}

ConstSymbol::~ConstSymbol() {
    // todo: should delete `value` and `conditions` here;
}

int ConstSymbol::getName() const {
    return name;
}

Type ConstSymbol::getType() const {
    return type;
}

Node *ConstSymbol::getValue() const {
    return value;
}

Node *ConstSymbol::getConditions() const {
    return conditions;
}

void ConstSymbol::dump() const {
    std::cout << "\tConstant: " << Token::name(name) << ", type: " << getTypeName(type) << std::endl;
}
