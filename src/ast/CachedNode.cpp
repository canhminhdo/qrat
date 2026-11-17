//
// Created by CanhDo on 2024/11/26.
//

#include "ast/CachedNode.hpp"
#include "ast/StmNode.hpp"
#include <cassert>
#include <iostream>

CachedNode::~CachedNode() {
    for (auto *node: nodes) {
        delete node;
    }
    nodes.clear();
    nodeTab.clear();
}

Node *CachedNode::makeNode(Node *node) {
    // a statement that is not shared is owned by its StmSeq; caching it too would delete it twice
    assert(dynamic_cast<StmNode *>(node) == nullptr || static_cast<StmNode *>(node)->isShared());
    auto it = nodeTab.find(node);
    if (it != nodeTab.end()) {
        // std::cout << "Found node in cache, then delete the newly created node\n";
        // node->dump();
        // std::cout << "-------------------\n";
        delete node;
        return nodes[it->second];
    }
    nodeTab[node] = nodes.size();
    nodes.push_back(node);
    return node;
}

void CachedNode::dump() const {
    std::cout << "CachedNode (expressions):\n";
    for (auto *node: nodes) {
        if (dynamic_cast<StmNode *>(node) != nullptr) {
            continue;
        }
        node->dump();
        std::cout << "-------------------\n";
    }
    std::cout << "CachedNode (statements):\n";
    for (auto *node: nodes) {
        if (dynamic_cast<StmNode *>(node) == nullptr) {
            continue;
        }
        node->dump();
        std::cout << "-------------------\n";
    }
}
