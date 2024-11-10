//
// Created by Canh Minh Do on 2024/11/10.
//

#ifndef QRAT_MACROS_HPP
#define QRAT_MACROS_HPP

#include <iostream>

using namespace std;

enum SpecialConstants {
    UNUSED = -1
};

#define Assert(condition, message)                                     \
    if (!(condition))                                                  \
    ((cerr << "ASSERT FAILED: " << __FILE__ << ':' << __LINE__ << '\n' \
           << message << endl),                                        \
     abort())

#endif//QRAT_MACROS_HPP
