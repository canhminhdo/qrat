//
// Created by CanhDo on 2024/11/20.
//

#ifndef STMSEQNODE_HPP
#define STMSEQNODE_HPP

#include <vector>

#include "ast/StmNode.hpp"

class StmSeqNode : public StmNode {
public:
    void addStm(StmNode *stm);

    void eval() override {
    };

    void dump() override;

private:
    std::vector<StmNode *> stmSeq;
};

#endif //STMSEQNODE_HPP
