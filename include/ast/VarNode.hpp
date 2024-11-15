//
// Created by CanhDo on 2024/11/15.
//

#ifndef VARNODE_HPP
#define VARNODE_HPP

#include "ast/Node.hpp"
#include "core/Token.hpp"
#include "core/type.hpp"

class VarNode : public Node {
public:
    VarNode(Token variable, Type type);
    void eval() override;
    int getName() const;
    const char *getTypeName() const;
    // for debugging
    void dump() const;

private:
    int name;
    Type type;
};

#endif//VARNODE_HPP
