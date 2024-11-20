//
// Created by Canh Minh Do on 2024/11/10.
//

#ifndef QRAT_TOKEN_HPP
#define QRAT_TOKEN_HPP

#include "utility/StringTable.hpp"

class Token {
public:
    void tokenize(const char *tokenString, int lineNumber);

    const char *name() const;

    int code() const;

    static const char *name(int code);

    static int code(const char *name);

    static void dump();

private:
    static StringTable stringTable;
    int codeNr;
    int lineNr;
};

using TokenList = std::vector<Token>;

#endif//QRAT_TOKEN_HPP
