//
// Created by CanhDo on 2024/11/20.
//

#ifndef SKIPSTMNODE_HPP
#define SKIPSTMNODE_HPP

#include "ast/StmNode.hpp"

class SkipStmNode : public StmNode {
public:
    void eval() override {
    };

    ~SkipStmNode() override = default;

    void dump(bool recursive = true) override;

    const char *getName() const;

    void info() override;

private:
    // no out-of-class definition required, ODR-usable (One Definition Rule)
    inline static const char *name = "skip";
};

inline const char *SkipStmNode::getName() const {
    return name;
}

#endif //SKIPSTMNODE_HPP
