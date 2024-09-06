//
// Created by CanhDo on 2024/08/20.
//

//#ifndef QRAT_GETCH_H
//#define QRAT_GETCH_H
#pragma once

#include <string>

namespace qrat {
    extern std::string buf; // buffer for unget
    /**
     * @brief Get a (possibly pushed back) character.
     * @return a character
     */
    int getch(void);
    /**
     * Push a character back on input buffer
     * @param c the character being pushed back
     */
    void ungetch(int c);
}
//#endif //QRAT_GETCH_H
