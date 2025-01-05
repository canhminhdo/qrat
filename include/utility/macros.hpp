//
// Created by Canh Minh Do on 2024/11/10.
//

#ifndef QRAT_MACROS_HPP
#define QRAT_MACROS_HPP

#include <iostream>

using namespace std;

enum DeclMode { NONE_DECL, VAR_DECL, CONST_DECL };

enum ErrorCode {
    NO_ERROR,
    SYNTAX_ERROR,
    SEMANTIC_ERROR,
    RUNTIME_ERROR,
    INTERNAL_ERROR,
};

enum SystemMode {
    INTERACTIVE_MODE,
    LOADING_FILE_MODE,
};

enum SpecialConstants {
    UNUSED = -1,
    UNBOUNDED = INT_MAX,
};

typedef long Int64;

#define local_inline extern __inline__

#define Assert(condition, message)                                     \
    if (!(condition))                                                  \
    ((cerr << "ASSERT FAILED: " << __FILE__ << ':' << __LINE__ << '\n' \
           << message << endl),                                        \
     abort())

#endif//QRAT_MACROS_HPP
