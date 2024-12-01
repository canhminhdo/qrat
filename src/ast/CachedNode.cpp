//
// Created by CanhDo on 2024/11/26.
//

#include "ast/CachedNode.hpp"
#include <iostream>

CachedNode::~CachedNode() {
    for (auto *node: nodes) {
        delete node;
    }
    nodes.clear();
    nodeTab.clear();
}

Node *CachedNode::makeNode(Node *node) {
    auto it = nodeTab.find(node);
    if (it != nodeTab.end()) {
        return nodes[it->second];
    }
    nodeTab[node] = nodes.size();
    nodes.push_back(node);
    return node;
}

void CachedNode::dump() const {
    for (auto *node: nodes) {
        node->dump();
    }
}
