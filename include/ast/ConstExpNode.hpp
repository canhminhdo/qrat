//
// Created by CanhDo on 2024/11/19.
//

#ifndef CONSTEXP_HPP
#define CONSTEXP_HPP

#include "ExpNode.hpp"
#include "core/ConstSymbol.hpp"

class ConstExpNode : public ExpNode {
public:
    ConstExpNode(Symbol *symbol);

    void eval() override {
    }

    void dump() override {
    }

private:
    ConstSymbol *symbol;
};

#endif //CONSTEXP_HPP
