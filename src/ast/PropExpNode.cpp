//
// Created by CanhDo on 2024/12/22.
//

#include "ast/PropExpNode.hpp"
#include "utility/HashUtil.hpp"
#include "core/Token.hpp"
#include <iostream>

PropExpNode::PropExpNode(Symbol *var, ExpNode *expr) : expr{expr} {
    vars.push_back(var);
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
