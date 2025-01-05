//
// Created by CanhDo on 2024/08/29.
//

#include "core/global.hpp"
#include "lexer.hpp"
#include "utility/macros.hpp"
#include <iostream>

void printVersion();
void printBanner();
void printHelp();

extern FILE *yyin;
extern int yyparse();
extern std::vector<char *> pendingFiles;
void handlePendingFiles(bool clearMemory = true);
bool handleCommandLine();
void yy_delete_buffer(YY_BUFFER_STATE buffer);
YY_BUFFER_STATE yy_scan_string(const char *str);
void yyrestart(FILE *input_file);

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
    handlePendingFiles(false);
    while (1) {
        if (!handleCommandLine())
            continue;
        handlePendingFiles();
    }
    return 0;
}

void handlePendingFiles(bool clearMemory) {
    systemMode = LOADING_FILE_MODE;
    for (int i = 0; i < pendingFiles.size(); i++) {
        if (!(yyin = fopen(pendingFiles[i], "r"))) {
            std::cerr << "Error: Opening file '" << pendingFiles[i] << "': " << strerror(errno) << std::endl;
            continue;
        }
        yyrestart(yyin);
        yyparse();
        fclose(yyin);
        if (clearMemory)
            delete[] pendingFiles[i];
    }
    pendingFiles.clear();
}

bool handleCommandLine() {
    systemMode = INTERACTIVE_MODE;
    std::string inputLine;
    std::cout << "qrat> " << std::flush;
    std::getline(std::cin, inputLine);
    if (std::cin.eof()) {// Check for EOF (Ctrl+D or Ctrl+Z)
        exit(0);
    }
    auto buffer = yy_scan_string(inputLine.c_str());
    if (!buffer) {
        std::cerr << "Error: Unable to scan the command." << std::endl;
        return false;
    }
    yyparse();
    yy_delete_buffer(buffer);
    return true;
}