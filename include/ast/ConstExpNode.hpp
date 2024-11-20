//
// Created by CanhDo on 2024/11/19.
//

#ifndef CONSTEXP_HPP
#define CONSTEXP_HPP

#include "ast/ExpNode.hpp"
#include "core/Symbol.hpp"

class ConstExpNode : public ExpNode {
public:
    ConstExpNode(Symbol *symbol);

    void eval() override {
    }

    void dump() override;

private:
    Symbol *symbol;
};

#endif //CONSTEXP_HPP
