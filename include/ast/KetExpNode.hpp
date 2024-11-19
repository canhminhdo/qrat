//
// Created by CanhDo on 2024/11/19.
//

#ifndef KETEXP_HPP
#define KETEXP_HPP

#include "ast/ExpNode.hpp"

enum class KetType {
    KET_ZERO,
    KET_ONE
};

class KetExpNode : public ExpNode {
public:
    KetExpNode(KetType type);

    KetType getType() const;

    const char *getName() const;

    void eval() override {
    }

    void dump() override {
    }

private:
    static const char *KetNames[];
    KetType type;
};

#endif //KETEXP_HPP
