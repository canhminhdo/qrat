//
// Created by CanhDo on 2024/11/15.
//

#ifndef ASTNODE_HPP
#define ASTNODE_HPP

class Node {
public:
    virtual ~Node() = default;

    virtual void eval() = 0;

    virtual void dump(bool recursive = true) = 0;
};

#endif //ASTNODE_HPP
