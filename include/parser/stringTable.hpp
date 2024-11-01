//
// Created by CanhDo on 2024/09/17.
//

#ifndef QRAT_STRING_TABLE_HPP
#define QRAT_STRING_TABLE_HPP

#include <string>
#include <unordered_map>

class StringTable {
public:
    // insert a string and return a pointer to the stored string
    const std::string* insert(const std::string& str);
    // look up a string in the table
    const std::string* lookup(const std::string& str) const;
    // print out the contents of the string table
    void dump() const;
private:
    std::unordered_map<std::string, std::string> table;
};

#endif
