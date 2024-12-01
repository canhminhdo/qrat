//
// Created by CanhDo on 2024/11/15.
//

#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <cstddef>

class Node {
public:
    virtual ~Node() = default;

    static constexpr int seed = 0;

    static std::size_t combinedHash(std::size_t hash1, std::size_t hash2);

    virtual void eval() = 0;

    virtual void dump() = 0;

    virtual std::size_t getHash() const = 0;

    virtual bool isEqual(const Node &other) const = 0;
};

inline std::size_t Node::combinedHash(std::size_t hash1, std::size_t hash2) {
    hash1 ^= hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
    return hash1;
}

#endif //ASTNODE_HPP
