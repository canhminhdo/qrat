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

    ~ConstExpNode() override = default;

    std::size_t getHash() const override;

    bool isEqual(const Node &other) const override;

    Symbol *getSymbol() const;

    void eval() override {
    }

    void dump(bool recursive = true) override;

private:
    Symbol *symbol;
};

#endif //CONSTEXP_HPP
