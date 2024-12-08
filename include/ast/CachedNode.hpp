//
// Created by CanhDo on 2024/11/26.
//

#ifndef CACHEDNODE_HPP
#define CACHEDNODE_HPP

#include "Node.hpp"
#include "core/Gate.hpp"
#include <unordered_map>
#include <vector>

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

    struct GateTypeHash {
        std::size_t operator()(const GateType type) const {
            return std::hash<int>()(static_cast<int>(type));
        }
    };

    struct GateTypeEqual {
        bool operator()(const GateType lhs, const GateType rhs) const {
            return lhs == rhs;
        }
    };

    ~CachedNode();

    Node *makeNode(Node *node);

    Gate *makeGate(Gate *gate);

    void dump() const;

private:
    std::unordered_map<Node *, int, NodeHash, NodeEqual> nodeTab;
    std::vector<Node *> nodes;
    std::unordered_map<GateType, int, GateTypeHash, GateTypeEqual> gateTab;
    std::vector<Gate *> gates;
};
#endif//CACHEDNODE_HPP
