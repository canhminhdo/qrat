//
// Created by CanhDo on 2025/05/09.
//

#ifndef PROPTABLE_HPP
#define PROPTABLE_HPP

#include "ast/ExpNode.hpp"
#include <unordered_map>
#include <vector>

class PropTable {
public:
    PropTable() = default;

    ~PropTable() = default;

    void addProp(int name, ExpNode *value = nullptr);

    ExpNode *lookup(int name);

    std::vector<ExpNode *> getProps() const;

    std::unordered_map<int, ExpNode *> getPropTab() const;

    void dump() const;

private:
    std::unordered_map<int, ExpNode *> propTab;
    std::vector<ExpNode *> props;
};

#endif //PROPTABLE_HPP
