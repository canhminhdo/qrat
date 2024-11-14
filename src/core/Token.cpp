//
// Created by CanhDo on 2024/11/14.
//

#include "core/Token.hpp"
#include "utility/StringTable.hpp"

StringTable Token::stringTable;

void Token::tokenize(const char *tokenString, int lineNumber) {
    codeNr = stringTable.encode(tokenString);
    lineNr = lineNumber;
}

const char *Token::name() {
    return stringTable.name(codeNr);
}

const void Token::dump() {
    stringTable.dump();
}

int Token::lookup(const char *name) {
    return stringTable.lookup(name);
}

