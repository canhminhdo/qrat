//
// Created by CanhDo on 2024/12/22.
//

#include "ast/PropExpNode.hpp"
#include "core/Token.hpp"
#include "utility/HashUtil.hpp"
#include <iostream>

PropExpNode::PropExpNode(Symbol *var, ExpNode *expr) : expr{expr} {
    vars.push_back(var);
}

PropExpNode::PropExpNode(Symbol *var1, Symbol *var2, ExpNode *expr) : expr{expr}{
    vars.push_back(var1);
    vars.push_back(var2);
}

bool PropExpNode::isEqual(const Node &other) const {
    const auto *otherProp = dynamic_cast<const PropExpNode *>(&other);
    if (otherProp == nullptr || vars.size() != otherProp->vars.size() || !expr->isEqual(*otherProp->expr))
        return false;
    for (int i = 0; i < vars.size(); i++) {
        if (vars[i] != otherProp->vars[i])
            return false;
    }
    return true;
}

std::vector<Symbol *> PropExpNode::getVars() const {
    return vars;
}

ExpNode *PropExpNode::getExpr() const {
    return expr;
}

size_t PropExpNode::getHash() const {
    auto hash = HashUtil::combinedHash(seed, std::hash<ExpNode *>{}(expr));
    for (auto var: vars) {
        hash = HashUtil::combinedHash(hash, std::hash<Symbol *>{}(var));
    }
    return hash;
}

void PropExpNode::dump(bool recursive) {
    std::cout << "P(";
    for (int i = 0; i < vars.size(); i++) {
        std::cout << Token::name(vars[i]->getName()) << ", ";
    }
    expr->dump();
    std::cout << ")\n";
}

void PropExpNode::info() {
    std::cout << "P(";
    for (int i = 0; i < vars.size(); i++) {
        std::cout << Token::name(vars[i]->getName()) << ", ";
    }
    expr->info();
    std::cout << ")";
}
