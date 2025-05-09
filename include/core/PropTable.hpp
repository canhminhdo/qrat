//
// Created by CanhDo on 2025/05/09.
//

#ifndef PROPTABLE_HPP
#define PROPTABLE_HPP

#include <ast/PropExpNode.hpp>
#include <unordered_map>
#include <vector>

class PropTable {
public:
    PropTable() = default;

    ~PropTable() = default;

    void addProp(int name, PropExpNode *value = nullptr);

    PropExpNode *lookup(int name);

    std::vector<PropExpNode *> getProps() const;

    std::unordered_map<int, PropExpNode *> getPropTab() const;

    void dump() const;

private:
    std::unordered_map<int, PropExpNode *> propTab;
    std::vector<PropExpNode *> props;
};

#endif //PROPTABLE_HPP
