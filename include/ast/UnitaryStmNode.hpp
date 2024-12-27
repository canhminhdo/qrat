//
// Created by CanhDo on 2024/11/20.
//

#ifndef UNITARYSTMNODE_HPP
#define UNITARYSTMNODE_HPP

#include "ast/StmNode.hpp"
#include "core/SyntaxProg.hpp"
#include "core/Symbol.hpp"
#include "ir/operations/OpType.hpp"

class UnitaryStmNode : public StmNode {
public:
    UnitaryStmNode(int name, qc::OpType type, std::vector<Symbol *> controls, std::vector<Symbol *> targets);

    ~UnitaryStmNode() override = default;

    void eval() override {
    };

    std::vector<Symbol *> getControls() const;

    std::vector<Symbol *> getTargets() const;

    qc::OpType getOpType() const;

    int getName() const;

    void dump(bool recursive = true) override;

private:
    int name;
    qc::OpType type;
    std::vector<Symbol *> controls;
    std::vector<Symbol *> targets;
};

#endif //UNITARYSTMNODE_HPP
