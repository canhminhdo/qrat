//
// Created by CanhDo on 2025/05/09.
//

#include "core/PropTable.hpp"
#include "core/Token.hpp"
#include <iostream>

void PropTable::addProp(int name, PropExpNode *value) {
    if (propTab.find(name) != propTab.end()) {
        std::cerr << "Error: " << Token::name(name) << " proposition already exists" << std::endl;
        exit(1);
    }
    propTab[name] = value;
    props.push_back(value);
}

PropExpNode * PropTable::lookup(int name) {
    if (propTab.find(name) == propTab.end()) {
        return nullptr;
    }
    return propTab[name];
}

std::vector<PropExpNode *> PropTable::getProps() const {
    return props;
}

std::unordered_map<int, PropExpNode *> PropTable::getPropTab() const {
    return propTab;
}

void PropTable::dump() const {
    for (auto &entry: propTab) {
        std::cout << "Proposition: " << Token::name(entry.first) << std::endl;
        entry.second->dump();
    }
}
