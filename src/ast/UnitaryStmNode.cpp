//
// Created by CanhDo on 2024/11/20.
//

#include <iostream>
#include "ast/UnitaryStmNode.hpp"

UnitaryStmNode::UnitaryStmNode(SyntaxProg *currentSyntaxProg,
                               TokenList *vars1, Gate *gate, TokenList *vars2) {
    if (vars1->size() != vars2->size()) {
        std::cerr << "Error: output variables and input variables must have the same size" << std::endl;
        exit(1);
    }

    if (vars1->size() != gate->getArgsNr()) {
        std::cerr << "Error: invalid number of variables for gate " << gate->getName() << std::endl;
        exit(1);
    }

    for (int i = 0; i < vars1->size(); i++) {
        if (vars1->at(i).code() != vars2->at(i).code()) {
            std::cerr << "Error: output variables and input variables must have the same order" << std::endl;
            exit(1);
        }
    }

    // check whether the variables exist
    for (int i = 0; i < vars1->size(); i++) {
        if (!currentSyntaxProg->hasVarSymbol(vars1->at(i))) {
            std::cerr << "Error: " << vars1->at(i).name() << " variable is undefined" << std::endl;
            exit(1);
        }
        vars.push_back(dynamic_cast<VarSymbol *>(currentSyntaxProg->lookup(vars1->at(i))));
    }
    Node *node = currentSyntaxProg->makeNode(new GateExpNode(gate, vars));
    gateExp = dynamic_cast<GateExpNode *>(node);
}

UnitaryStmNode::~UnitaryStmNode() {
    delete gateExp;
}

std::size_t UnitaryStmNode::getHash() const {
    auto hash = combinedHash(seed, std::hash<GateExpNode *>{}(gateExp));
    for (auto var: vars) {
        hash = combinedHash(hash, std::hash<Symbol *>{}(var));
    }
    return hash;
}

bool UnitaryStmNode::isEqual(const Node &other) const {
    const UnitaryStmNode *otherUnitary = dynamic_cast<const UnitaryStmNode *>(&other);
    if (otherUnitary == nullptr || gateExp != otherUnitary->getGateExp())
        return false;
    const auto &vars2 = otherUnitary->getVars();
    if (vars.size() != vars2.size())
        return false;
    for (int i = 0; i < vars.size(); i++) {
        if (vars[i] != vars2[i])
            return false;
    }
    return true;
}

GateExpNode *UnitaryStmNode::getGateExp() const {
    return gateExp;
}

const std::vector<Symbol *> &UnitaryStmNode::getVars() const {
    return vars;
}

void UnitaryStmNode::dump() {
    for (int i = 0; i < vars.size(); i++) {
        std::cout << Token::name(vars[i]->getName()) << ((i < vars.size() - 1) ? ", " : "");
    }
    std::cout << " = ";
    gateExp->dump();
}
