//
// Created by CanhDo on 2024/11/19.
//

#ifndef KETEXP_HPP
#define KETEXP_HPP

#include "ast/ExpNode.hpp"

// ODR-usable enum class
enum class KetType {
    // one-qubit states
    KET_ZERO,
    KET_ONE,
    KET_PLUS,
    KET_MINUS,
    KET_RANDOM,
    // two-qubit states
    KET_PHI_PLUS,
    KET_PHI_MINUS,
    KET_PSI_PLUS,
    KET_PSI_MINUS,
};

// inline initialization with ODR
inline static const char *KetNames[] = {"|0>", "|1>", "|+>", "|->", "random", "|phi+>", "|phi->", "|psi+>", "|psi->"};

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

    void dump(bool recursive) override;

    void info() override;

private:
    KetType type;
};

#endif //KETEXP_HPP
