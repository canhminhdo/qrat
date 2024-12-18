//
// Created by CanhDo on 2024/11/19.
//

#ifndef EXPNODE_HPP
#define EXPNODE_HPP

#include "ast/Node.hpp"
#include <cstddef>

class ExpNode : public Node {
public:
    static constexpr int seed = 0;

    virtual std::size_t getHash() const = 0;

    virtual bool isEqual(const Node &other) const = 0;
};

#endif //EXPNODE_HPP
