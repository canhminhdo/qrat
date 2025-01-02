//
// Created by CanhDo on 2024/11/20.
//

#ifndef UNITARYSTMNODE_HPP
#define UNITARYSTMNODE_HPP

#include "ast/StmNode.hpp"
#include "core/SyntaxProg.hpp"
#include "core/Symbol.hpp"
#include "ir/operations/OpType.hpp"
#include "Definitions.hpp"

class UnitaryStmNode : public StmNode {
public:
    UnitaryStmNode(int name, qc::OpType type, std::vector<Symbol *> controls, std::vector<Symbol *> targets,
                   std::vector<qc::fp> parameters = {});

    ~UnitaryStmNode();

    void eval() override {
    };

    std::vector<Symbol *> getControls() const;

    std::vector<Symbol *> getTargets() const;

    std::vector<qc::fp> getParams() const;

    qc::OpType getOpType() const;

    int getName() const;

    void dump(bool recursive = true) override;

    void info() override;

private:
    int name;
    qc::OpType type;
    std::vector<Symbol *> controls;
    std::vector<Symbol *> targets;
    std::vector<qc::fp> parameters;
};

#endif //UNITARYSTMNODE_HPP
