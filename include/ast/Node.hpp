//
// Created by CanhDo on 2024/11/15.
//

#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <cstddef>

class Node {
public:
    virtual ~Node() = default;

    virtual std::size_t getHash() const = 0;

    virtual bool isEqual(const Node &other) const = 0;

    virtual void eval() = 0;

    virtual void dump(bool recursive = true) = 0;

    virtual void info() = 0;
};

#endif //ASTNODE_HPP
