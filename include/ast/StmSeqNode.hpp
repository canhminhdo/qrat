//
// Created by CanhDo on 2024/11/20.
//

#ifndef STMSEQNODE_HPP
#define STMSEQNODE_HPP

#include <vector>

#include "ast/StmNode.hpp"

class StmSeqNode : public StmNode {
public:
    ~StmSeqNode() override = default;

    void addStm(StmNode *stm);

    std::size_t getHash() const override;

    bool isEqual(const Node &other) const override;

    const std::vector<StmNode *> &getStmSeq() const;

    void eval() override {
    };

    void dump() override;

private:
    std::vector<StmNode *> stmSeq;
};

#endif //STMSEQNODE_HPP
