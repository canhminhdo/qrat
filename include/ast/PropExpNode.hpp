//
// Created by CanhDo on 2024/12/22.
//

#ifndef PROPEXPNODE_HPP
#define PROPEXPNODE_HPP

#include "ExpNode.hpp"
#include "core/Symbol.hpp"
#include <vector>

class PropExpNode : public ExpNode {
public:
    PropExpNode(Symbol *var, ExpNode *expr);

    PropExpNode(Symbol *var1, Symbol *var2, ExpNode *expr);

    ~PropExpNode() override = default;

    size_t getHash() const override;

    bool isEqual(const Node &other) const override;

    std::vector<Symbol *> getVars() const;

    ExpNode *getExpr() const;

    void eval() override {
    }

    void dump(bool recursive = true) override;

    void info() override;

private:
    std::vector<Symbol *> vars;
    ExpNode *expr;
};
#endif //PROPEXPNODE_HPP
