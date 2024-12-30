//
// Created by CanhDo on 2024/12/22.
//

#ifndef INITEXPNODE_HPP
#define INITEXPNODE_HPP

#include "ExpNode.hpp"
#include "core/Symbol.hpp"
#include <string>

class InitExpNode : public ExpNode {
public:
    explicit InitExpNode(Symbol *var);

    bool isEqual(const Node &other) const override;

    size_t getHash() const override;

    Symbol *getVar() const;

    void eval() override {
    }

    const char *getName() const;

    void dump(bool recursive = true) override;

    void info() override;

private:
    Symbol *var;
    inline static const char *name = "init";
};

inline const char *InitExpNode::getName() const {
    return name;
}
#endif //INITEXPNODE_HPP
