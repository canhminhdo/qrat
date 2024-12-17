//
// Created by CanhDo on 2024/11/19.
//

#ifndef KETEXP_HPP
#define KETEXP_HPP

#include "ast/ExpNode.hpp"

// ODR-usable enum class
enum class KetType {
    KET_ZERO,
    KET_ONE,
    KET_RANDOM
};

// inline initialization with ODR
inline static const char *KetNames[] = {"|0>", "|1>", "random"};

class KetExpNode : public ExpNode {
public:
    KetExpNode(KetType type);

    ~KetExpNode() override = default;

    KetType getType() const;

    const char *getName() const;

    std::size_t getHash() const override;

    bool isEqual(const Node &other) const override;

    void eval() override {
    }

    void dump() override;

private:
    KetType type;
};

#endif //KETEXP_HPP
