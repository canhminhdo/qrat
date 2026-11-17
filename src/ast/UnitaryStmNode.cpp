//
// Created by CanhDo on 2024/11/20.
//

#include "ast/UnitaryStmNode.hpp"
#include "utility/HashUtil.hpp"
#include <functional>
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

std::size_t UnitaryStmNode::getHash() const {
    auto hash = HashUtil::combinedHash(std::hash<int>{}(name), std::hash<int>{}(static_cast<int>(type)));
    for (auto *control: controls) {
        hash = HashUtil::combinedHash(hash, std::hash<Symbol *>{}(control));
    }
    // keeps controls and targets apart: CU[a, b] with one control differs from a gate on targets a, b
    hash = HashUtil::combinedHash(hash, controls.size());
    for (auto *target: targets) {
        hash = HashUtil::combinedHash(hash, std::hash<Symbol *>{}(target));
    }
    for (auto param: parameters) {
        // -0.0 == 0.0, so both must hash alike
        hash = HashUtil::combinedHash(hash, std::hash<qc::fp>{}(param == 0 ? 0 : param));
    }
    return hash;
}

bool UnitaryStmNode::isEqual(const Node &other) const {
    const auto *otherUnitary = dynamic_cast<const UnitaryStmNode *>(&other);
    if (otherUnitary == nullptr) {
        return false;
    }
    return name == otherUnitary->name && type == otherUnitary->type && controls == otherUnitary->controls &&
           targets == otherUnitary->targets && parameters == otherUnitary->parameters;
}

bool UnitaryStmNode::isShared() const {
    return true;
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
