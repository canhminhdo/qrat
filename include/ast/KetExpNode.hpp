//
// Created by CanhDo on 2024/11/19.
//

#ifndef KETEXP_HPP
#define KETEXP_HPP

#include "ast/ExpNode.hpp"

// ODR-usable enum class
enum class KetType {
    KET_ZERO,
    KET_ONE
};

// inline initialization with ODR
inline static const char *KetNames[] = {"|0>", "|1>"};

class KetExpNode : public ExpNode {
public:
    KetExpNode(KetType type);

    KetType getType() const;

    const char *getName() const;

    void eval() override {
    }

    void dump() override;

private:
    KetType type;
};

#endif //KETEXP_HPP
