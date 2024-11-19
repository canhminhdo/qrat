//
// Created by CanhDo on 2024/11/19.
//

#include "ast/ConstExpNode.hpp"

ConstExpNode::ConstExpNode(Symbol *symbol) : symbol(dynamic_cast<ConstSymbol *>(symbol)) {
}