//
// Created by CanhDo on 2024/11/15.
//

#ifndef CONSTNODE_HPP
#define CONSTNODE_HPP

#include "Node.hpp"
#include "core/Token.hpp"
#include "core/type.hpp"

class ConstNode : public Node {
public:
    ConstNode(Token constant, Type constType);
    void eval() override;
    const char *getTypeName() const;
    // for debugging
    void dump() const;

private:
    int name;
    Type type;
};

#endif//CONSTNODE_HPP
