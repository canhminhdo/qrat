//
// Created by CanhDo on 2024/11/19.
//

#ifndef SYMBOL_HPP
#define SYMBOL_HPP
#include <ostream>

class Symbol {
public:
    virtual int getName() const = 0;
    virtual void dump() const = 0;
};

#endif//SYMBOL_HPP
