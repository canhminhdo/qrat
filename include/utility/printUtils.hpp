//
// Created by CanhDo on 2025/01/07.
//

#ifndef PRINTUTILS_HPP
#define PRINTUTILS_HPP

#include <string>

void printVersion();
void printBanner();
void printHelp();
const char * isFlag(const char *arg, const char *flag);
void capitalizeFirstLetter(std::string &str);
void capitalizeLetters(std::string &str);

#endif //PRINTUTILS_HPP
