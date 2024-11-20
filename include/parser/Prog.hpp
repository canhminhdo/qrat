//
// Created by CanhDo on 2024/09/17.
//

#ifndef QRAT_PROG_HPP
#define QRAT_PROG_HPP

#include <string>

class PROG {
public:
    // start program declaration with a given name
    void begin(std::string name);

    void addVariables();

    void addConstants();

    void addInit(Token);

    void addStatments();

    // end program declaration
    void end();

private:
    std::string *name;
};
#endif//QRAT_PROG_HPP
