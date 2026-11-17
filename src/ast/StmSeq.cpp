//
// Created by CanhDo on 2024/11/20.
//

#include "ast/StmSeq.hpp"
#include <iostream>

StmSeq::~StmSeq() {
    for (auto *stm: stms) {
        // shared statements belong to the program's CachedNode
        if (!stm->isShared()) {
            delete stm;
        }
    }
}

void StmSeq::addStm(StmNode *stm) {
    stms.push_back(stm);
}

const std::vector<StmNode *> &StmSeq::getStms() const {
    return stms;
}

bool StmSeq::empty() const {
    return stms.empty();
}

int StmSeq::size() const {
    return static_cast<int>(stms.size());
}

void StmSeq::dump(bool recursive) {
    for (auto *stm: stms) {
        stm->dump(recursive);
    }
    std::cout << "End" << std::endl;
}

void StmSeq::info() {
    for (auto *stm: stms) {
        stm->info();
    }
}
