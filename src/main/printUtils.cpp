//
// Created by CanhDo on 2025/01/03.
//

#include "config.h"
#include "utility/Tty.hpp"
#include <iostream>
#include <sys/time.h>

const char * isFlag(const char *arg, const char *flag) {
    char f;
    do {
        f = *flag++;
        if (f == 0)
            return arg;
    } while (f == *arg++);
    return 0;
}

void capitalizeFirstLetter(std::string &str) {
    if (!str.empty() && std::islower(str[0])) {
        str[0] = std::toupper(str[0]);
    }
}

void capitalizeLetters(std::string &str) {
    for (char &c: str) {
        if (std::islower(c)) {
            c = std::toupper(c);
        }
    }
}

void printVersion() {
    std::cout << PROJECT_VERSION << std::endl;
    exit(0);
}

void printHelp() {
    std::cout << PROJECT_DESCRIPTION << "\n"
              << "Usage: " << PROJECT_NAME << " [options] [files]\n"
              << "Options:\n"
              << "  --help\t\tDisplay this information\n"
              << "  --version\t\tDisplay version number\n"
              << "  -no-banner\t\tDo not output banner on startup\n"
              << "  -random-seed=<int>\t\tSet seed for random number generator\n"
              << "\n"
              << "Send bug reports to: " << "canhdo@jaist.ac.jp" << std::endl;
    exit(0);
}

void printBanner() {
    struct timeval t;
    gettimeofday(&t, 0);
    time_t secs = t.tv_sec;
    std::string projectName = PROJECT_NAME;
    capitalizeLetters(projectName);

    std::cout << "\t\t     \\|||||||||||||||||/\n";
    std::cout << "\t\t   --- Welcome to " << Tty(Tty::RED) << 'Q' << Tty(Tty::BLUE) << 'R' << Tty(Tty::GREEN) << 'A' << Tty(Tty::YELLOW) << 'T' << Tty(Tty::RESET) << " ---\n";
    std::cout << "\t\t     /|||||||||||||||||\\\n";
    std::cout << "\t     " << projectName << " " << PROJECT_VERSION << " built: " << PROJECT_BUILD_DATE << '\n';
    std::cout << "\t        Copyright 2025 Ogata Laboratory\n";
    std::cout << "\t\t   " << ctime(&secs);
}