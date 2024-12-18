//
// Created by CanhDo on 2024/11/20.
//

#ifndef GATEEXPNODE_HPP
#define GATEEXPNODE_HPP

#include <vector>

#include "ast/ExpNode.hpp"
#include "core/Gate.hpp"
#include "core/VarSymbol.hpp"

class GateExpNode : public ExpNode {
public:
    GateExpNode(Gate *gate, std::vector<Symbol *> vars);

    ~GateExpNode() override = default;

    void eval() override {
    }

    std::size_t getHash() const override;

    Gate *getGate() const;

    bool isEqual(const Node &other) const override;;

    void dump(bool recursive = false) override;

private:
    Gate *gate;
    std::vector<Symbol *> vars;
};
#endif //GATEEXPNODE_HPP
