//
// Created by CanhDo on 2024/08/29.
//

#include <iostream>
#include "config.h"
#include "utility/Tty.hpp"

extern FILE *yyin;
extern int yyparse();
extern std::vector<char *> pendingFiles;

void printVersion();
void printBanner();
void printHelp();
void yy_scan_string(const char *str);

int main(int argc, char *argv[]) {
    bool outputBanner = true;
    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (arg[0] == '-') {
            if (strcmp(arg, "--help") == 0)
                printHelp();
            else if (strcmp(arg, "--version") == 0)
                printVersion();
            else if (strcmp(arg, "-no-banner") == 0)
                outputBanner = false;
        } else {
            pendingFiles.push_back(arg);
        }
    }
    if (outputBanner)
        printBanner();

    for (int i = 0; i < pendingFiles.size(); i++) {
        if (!(yyin = fopen(pendingFiles[i], "r"))) {
            fprintf(stderr, "Error opening file '%s': %s\n", pendingFiles[i], strerror(errno));
            continue;
        }
        yyparse();
        fclose(yyin);
    }
    yyin = stdin; // Set input stream to stdin
    std::string inputLine;
    while (1) {
        std::cout << "Qrat> " << std::flush;
        std::getline(std::cin, inputLine);
        if (std::cin.eof()) {  // Check for EOF (Ctrl+D or Ctrl+Z)
            break;
        }
        yy_scan_string(inputLine.c_str());
        yyparse();
    }
    // reading from a file for testing
//    const char* fileName = "prog-loop.qw";
//    if (!(yyin = fopen(fileName, "r"))) {
//        fprintf(stderr, "Error opening file '%s': %s\n", fileName, strerror(errno));
//        return 1;
//    }
//    yyparse();
//    fclose(yyin);
    return 0;
}