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

ExpNode *CachedNode::makeNode(ExpNode *node) {
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
    std::cout << "CachedNode:\n";
    for (auto *node: nodes) {
        node->dump();
        std::cout << "-------------------\n";
    }
}
