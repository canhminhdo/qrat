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

const char *Token::name() const {
    return stringTable.name(codeNr);
}

const char *Token::name(int code) {
    return stringTable.name(code);
}

int Token::code() const {
    return codeNr;
}

int Token::code(const char *name) {
    return stringTable.lookup(name);
}

void Token::dump() {
    stringTable.dump();
}
