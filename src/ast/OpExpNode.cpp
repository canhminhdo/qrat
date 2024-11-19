//
// Created by CanhDo on 2024/11/19.
//

#include "ast/OpExpNode.hpp"

OpExpNode::OpExpNode(OpExpType type, ExpNode *left, ExpNode *right) : type{type}, left{left}, right{right} {
}
