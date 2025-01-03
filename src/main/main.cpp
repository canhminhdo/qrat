//
// Created by CanhDo on 2024/08/29.
//

#include "lexer.hpp"
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
void yy_switch_to_buffer(YY_BUFFER_STATE new_buffer);
YY_BUFFER_STATE yy_create_buffer(FILE *file, int size);

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
    for (int i = 0; i < pendingFiles.size(); i++) {
        if (!(yyin = fopen(pendingFiles[i], "r"))) {
            std::cerr << "Error: Opening file '" << pendingFiles[i] << "': " << strerror(errno) << std::endl;
            continue;
        }
        YY_BUFFER_STATE file_buffer = yy_create_buffer(yyin, YY_BUF_SIZE);
        yy_switch_to_buffer(file_buffer);
        yyparse();
        yy_delete_buffer(file_buffer);
        fclose(yyin);
        if (clearMemory)
            delete[] pendingFiles[i];
    }
    pendingFiles.clear();
}

bool handleCommandLine() {
    std::string inputLine;
    std::cout << "Qrat> " << std::flush;
    std::getline(std::cin, inputLine);
    if (std::cin.eof()) { // Check for EOF (Ctrl+D or Ctrl+Z)
        exit(0);
    }
    auto buffer = yy_scan_string(inputLine.c_str());
    yy_switch_to_buffer(buffer);
    if (!buffer) {
        std::cerr << "Error: Unable to scan the command." << std::endl;
        return false;
    }
    yyparse();
    yy_delete_buffer(buffer);
    return true;
}