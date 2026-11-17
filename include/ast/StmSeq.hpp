//
// Created by CanhDo on 2024/11/20.
//

#ifndef STMSEQ_HPP
#define STMSEQ_HPP

#include "ast/StmNode.hpp"
#include <vector>

class StmSeq {
public:
    StmSeq() = default;

    ~StmSeq();

    void addStm(StmNode *stm);

    const std::vector<StmNode *> &getStms() const;

    bool empty() const;

    int size() const;

    void dump(bool recursive = true);

    void info();

private:
    std::vector<StmNode *> stms;
};

#endif //STMSEQ_HPP
