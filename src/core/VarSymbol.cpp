//
// Created by CanhDo on 2024/11/19.
//

#include "core/VarSymbol.hpp"
#include "core/Token.hpp"
#include <iostream>

VarSymbol::VarSymbol(int name, Type type, Node *value) : name(name), type(type), value(value) {
}

int VarSymbol::getName() const {
    return name;
}

Type VarSymbol::getType() const {
    return type;
}

void VarSymbol::setValue(Node *value) {
    this->value = value;
}

Node *VarSymbol::getValue() {
    return value;
}

void VarSymbol::dump() const {
    std::cout << "\tVariable: " << Token::name(name) << ", type: " << getTypeName(type) << std::endl;
    value->dump();
}
