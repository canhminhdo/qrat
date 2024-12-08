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
        std::cout << "Found node in cache, then delete the newly created node\n";
        node->dump();
        std::cout << "-------------------\n";
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
    std::cout << "CachedGate:\n";
    for (auto *gate: gates) {
        gate->dump();
        std::cout << "-------------------\n";
    }
}

Gate *CachedNode::makeGate(Gate *gate) {
    auto it = gateTab.find(gate->getType());
    if (it != gateTab.end()) {
        std::cout << "Found gate in cache, then delete the newly created gate\n";
        delete gate;
        return gates[it->second];
    }
    GateType gType = gate->getType();
    gateTab[gType] = gates.size();
    gates.push_back(gate);
    return gate;
}
