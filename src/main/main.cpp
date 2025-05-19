//
// Created by CanhDo on 2024/08/29.
//

#include <Configuration.hpp>

#include "core/global.hpp"
#include "dd/DDOperation.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "utility/macros.hpp"
#include "utility/printUtils.hpp"
#include <iostream>
#include <utility/Tty.hpp>

extern std::vector<char *> pendingFiles;
void handlePendingFiles(bool clearMemory = true);
bool handleCommandLine();

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (arg[0] == '-') {
            if (strcmp(arg, "--help") == 0)
                printHelp();
            else if (strcmp(arg, "--version") == 0)
                printVersion();
            else if (strcmp(arg, "-no-banner") == 0)
                Configuration::outputBanner = false;
            else if (strcmp(arg, "-no-color") == 0)
                Tty::setEscapeSequencesAllowed(false);
            else if (const char *s = isFlag(arg, "-random-seed="))
                Configuration::seed = strtoul(s, nullptr, 0);
        } else {
            pendingFiles.push_back(arg);
        }
    }
    if (Configuration::outputBanner)
        printBanner();
    handlePendingFiles(false);
    while (true) {
        if (!handleCommandLine())
            continue;
        handlePendingFiles();
    }
    return 0;
}

void handlePendingFiles(bool clearMemory) {
    Configuration::systemMode = LOADING_FILE_MODE;
    for (auto &pendingFile: pendingFiles) {
        if (!(yyin = fopen(pendingFile, "r"))) {
            std::cerr << "Error: Opening file '" << pendingFile << "': " << strerror(errno) << std::endl;
            continue;
        }
        yyrestart(yyin);
        yyparse();
        fclose(yyin);
        if (clearMemory)
            delete[] pendingFile;
    }
    pendingFiles.clear();
}

bool handleCommandLine() {
    Configuration::systemMode = INTERACTIVE_MODE;
    std::string inputLine;
    std::cout << "qcheck> " << std::flush;
    std::getline(std::cin, inputLine);
    if (std::cin.eof()) {// Check for EOF (Ctrl+D or Ctrl+Z)
        exit(0);
    }
    auto buffer = yy_scan_string(inputLine.c_str());
    if (!buffer) {
        std::cerr << "Error: Unable to scan the command" << std::endl;
        return false;
    }
    yyparse();
    yy_delete_buffer(buffer);
    return true;
}