//
// Created by Canh Minh Do on 2024/11/10.
//

#include "utility/StringTable.hpp"
#include "utility/macros.hpp"
#include <cassert>
#include <iostream>

inline int
StringTable::hash(const char *name) {
    int h = 0;
    for (const char *p = name; *p; p++) {
        h = (3 * h) + *p;
    }
    return h;
}

inline int
StringTable::hash2(const char *name) {// 2nd hash function always returns an odd value
    int h = 0;
    for (const char *p = name; *p; p++) {
        h = (5 * h) ^ *p;
    }
    return h | 1;
}

StringTable::StringTable(int size) {
    int s = 2;
    while (s < size)
        s *= 2;
    s *= 2;
    hashTable.reserve(s);
    for (int i = 0; i < s; i++) {
        hashTable.push_back(UNUSED);
    }
    Assert(hashTable.capacity() == hashTable.size(), "the capacity and size are the same");
}

StringTable::~StringTable() {
    int nrStrings = stringTable.size();
    for (int i = 0; i < nrStrings; i++)
        delete[] stringTable[i];
}

int StringTable::encode(const char *name) {
    int mask = hashTable.capacity() - 1;
    int code;
    int step = 0;
    // h = (h1 + i * h2) % m for i = 0, 1, ...
    for (int i = hash(name);; i += step) {
        i &= mask;
        code = hashTable[i];
        if (code == UNUSED) {
            code = stringTable.size();
            stringTable.push_back(strcpy(new char[strlen(name) + 1], name));
            // resizing when the size of stringTable is half the hash table's capacity
            if (2 * (code + 1) > hashTable.capacity()) {
                resize();
            } else {
                hashTable[i] = code;
            }
            break;
        }
        if (strcmp(name, stringTable[code]) == 0)
            break;
        if (step == 0)
            step = hash2(name);// hash2(name) & mask ???
    }
    return code;
}

int StringTable::lookup(const char *name) const {
    int mask = hashTable.capacity() - 1;
    int step = 0;
    int code;
    // h = (h1 + i * h2) % m for i = 0, 1, ...
    for (int i = hash(name);; i += step) {
        i &= mask;
        code = hashTable[i];
        // not encoded yet
        if (code == UNUSED)
            return UNUSED;
        // check the same key
        if (strcmp(name, stringTable[code]) == 0)
            break;
        // the hash function h ensures that an unused slot is found if the name has not yet been encoded, provided that half of the hash table are empty
        if (step == 0)
            step = hash2(name);
    }
    return code;
}

void StringTable::resize() {
    int h = 2 * hashTable.capacity();
    hashTable.reserve(h);
    for (int i = 0; i < h; i++) {
        hashTable.push_back(UNUSED);
    }
    assert(hashTable.capacity() == hashTable.size());
    int mask = h - 1;
    int nrStrings = stringTable.size();
    for (int i = 0; i < nrStrings; i++) {
        char *s = stringTable[i];
        int j = hash(s) & mask;
        if (hashTable[j] != UNUSED) {
            int step = hash2(s);// hash2(s) & mask ???
            do
                j = (j + step) & mask;
            while (hashTable[j] != UNUSED);
        }
        hashTable[j] = i;
    }
}

void StringTable::dump() const {
    std::cout << "String Table:" << std::endl;
    for (int i = 0, nrStrings = stringTable.size(); i < nrStrings; i++) {
        std::cout << "\tcodeNr: " << i << ", string: " << stringTable[i] << std::endl;
    }
}
