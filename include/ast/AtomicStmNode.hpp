//
// Created by CanhDo on 2025/06/04.
//

#ifndef ATOMICSTMNODE_HPP
#define ATOMICSTMNODE_HPP

#include "ast/StmNode.hpp"
#include "ast/StmSeq.hpp"

class AtomicStmNode : public StmNode {
public:
    explicit AtomicStmNode(StmSeq *body);

    ~AtomicStmNode();

    StmSeq *getBody() const;

    bool isValid() const;

    void eval() override {
    };

    void dump(bool recursive) override;

    void info() override;
private:
    StmSeq *body;
};

#endif //ATOMICSTMNODE_HPP
