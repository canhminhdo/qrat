//
// Created by CanhDo on 2024/09/17.
//

#include "parser/stringTable.hpp"
#include <iostream>

const std::string* StringTable::insert(const std::string& str) {
    auto result = table.emplace(str, str);
    // result.first is a point to pair<key, value>
    return &result.first->second;
}

const std::string* StringTable::lookup(const std::string& str) const {
    auto it = table.find(str);
    if (it != table.end()) {
        return &it->second;
    }
    return nullptr;
}

void StringTable::dump() const {
    for (auto& entry : table) {
        std::cout << "key: " << entry.first << ", value: " << entry.second << std::endl;
    }
}
