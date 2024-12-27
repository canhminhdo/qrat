//
// Created by CanhDo on 2024/11/26.
//

#ifndef CACHEDNODE_HPP
#define CACHEDNODE_HPP

#include "Node.hpp"
#include <unordered_map>
#include <vector>

#include "ExpNode.hpp"

class CachedNode {
public:
    struct NodeHash {
        std::size_t operator()(const ExpNode *node) const {
            return node->getHash();
        }
    };

    struct NodeEqual {
        bool operator()(const ExpNode *lhs, const ExpNode *rhs) const {
            return lhs->isEqual(*rhs);
        }
    };

    ~CachedNode();

    ExpNode *makeNode(ExpNode *node);

    void dump() const;

private:
    std::unordered_map<ExpNode *, int, NodeHash, NodeEqual> nodeTab;
    std::vector<ExpNode *> nodes;
};
#endif//CACHEDNODE_HPP
