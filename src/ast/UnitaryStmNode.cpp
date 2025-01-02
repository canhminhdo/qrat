//
// Created by CanhDo on 2024/11/20.
//

#include "ast/UnitaryStmNode.hpp"
#include <iostream>

UnitaryStmNode::UnitaryStmNode(int name, qc::OpType type, std::vector<Symbol *> controls,
                               std::vector<Symbol *> targets, std::vector<qc::fp> parameters) : name{name}, type{type}, controls{controls},
                                                                                                targets{targets}, parameters{parameters} {
}

UnitaryStmNode::~UnitaryStmNode() {
    controls.clear();
    targets.clear();
    parameters.clear();
}

std::vector<Symbol *> UnitaryStmNode::getControls() const {
    return controls;
}

std::vector<Symbol *> UnitaryStmNode::getTargets() const {
    return targets;
}

std::vector<qc::fp> UnitaryStmNode::getParams() const {
    return parameters;
}

qc::OpType UnitaryStmNode::getOpType() const {
    return type;
}

int UnitaryStmNode::getName() const {
    return name;
}

void UnitaryStmNode::dump(bool recursive) {
    std::cout << "---------------------------------" << std::endl;
    std::cout << "opType: " << qc::toString(type) << std::endl;
    std::cout << "#controls: " << controls.size() << std::endl;
    std::cout << "#targets: " << targets.size() << std::endl;
    std::vector<Symbol *> vars(controls);
    vars.insert(vars.end(), targets.begin(), targets.end());
    for (int i = 0; i < vars.size(); i++) {
        std::cout << Token::name(vars[i]->getName()) << ((i < vars.size() - 1) ? ", " : "");
    }
    std::cout << " = " << Token::name(name) << "[";
    for (int i = 0; i < vars.size(); i++) {
        std::cout << Token::name(vars[i]->getName()) << ((i < vars.size() - 1) ? ", " : "");
    }
    std::cout << "]" << std::endl;
    std::cout << "---------------------------------" << std::endl;
    if (recursive && next)
        next->dump();
}

void UnitaryStmNode::info() {
    std::vector<Symbol *> vars(controls);
    vars.insert(vars.end(), targets.begin(), targets.end());
    for (int i = 0; i < vars.size(); i++) {
        std::cout << Token::name(vars[i]->getName()) << ((i < vars.size() - 1) ? ", " : "");
    }
    std::cout << " = " << Token::name(name) << "[";
    for (int i = 0; i < vars.size(); i++) {
        std::cout << Token::name(vars[i]->getName()) << ((i < vars.size() - 1) ? ", " : "");
    }
    std::cout << "];";
}
