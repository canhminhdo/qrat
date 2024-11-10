//
// Created by Canh Minh Do on 2024/11/10.
//

#ifndef QRAT_STRINGTABLE_HPP
#define QRAT_STRINGTABLE_HPP

#include <vector>

class StringTable {
public:
    StringTable(int size = 1024);
    ~StringTable();
    int encode(const char *name);
    const char *name(int code) const;
    void dump();

private:
    static int hash(const char *name);
    static int hash2(const char *name);
    void resize();
    std::vector<char *> stringTable;
    std::vector<int> hashTable;
};

inline const char *
StringTable::name(int code) const {
    return stringTable[code];
}

#endif//QRAT_STRINGTABLE_HPP
