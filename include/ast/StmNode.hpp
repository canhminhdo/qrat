//
// Created by CanhDo on 2024/11/19.
//

#ifndef STMNODE_HPP
#define STMNODE_HPP

#include "ast/Node.hpp"
#include <cstddef>
#include <functional>

class StmNode : public Node {
public:
    std::size_t getHash() const override;

    bool isEqual(const Node &other) const override;

    virtual bool isShared() const;
};

inline std::size_t StmNode::getHash() const {
    return std::hash<const StmNode *>{}(this);
}

inline bool StmNode::isEqual(const Node &other) const {
    return this == &other;
}

inline bool StmNode::isShared() const {
    return false;
}

#endif //STMNODE_HPP
