//
// Created by Canh Minh Do on 2024/11/10.
//

#include "parser/Token.hpp"

void Token::tokenize(const char *tokenString, int lineNumber) {
    codeNr = stringTable.encode(tokenString);
    lineNr = lineNumber;
}