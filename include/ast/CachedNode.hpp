//
// Created by CanhDo on 2024/11/26.
//

#ifndef CACHEDNODE_HPP
#define CACHEDNODE_HPP

#include <unordered_map>
#include <vector>
#include "Node.hpp"

class CachedNode {
public:
    struct NodeHash {
        std::size_t operator()(const Node *node) const {
            return node->getHash();
        }
    };

    struct NodeEqual {
        bool operator()(const Node *lhs, const Node *rhs) const {
            return lhs->isEqual(*rhs);
        }
    };

    ~CachedNode();

    Node *makeNode(Node *node);

    void dump() const;

private:
    std::unordered_map<Node *, int, NodeHash, NodeEqual> nodeTab;
    std::vector<Node *> nodes;
};
#endif //CACHEDNODE_HPP
