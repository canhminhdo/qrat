//
// Created by Canh Minh Do on 2025/05/11.
//

#include "model/PrismRunner.hpp"
#include <iostream>
#include <unistd.h>

void PrismRunner::modelCheck(const std::string &modelFile, const std::string &property) {
    std::string prismBin = getPrismPath();
    std::string cmd = prismBin + " -javastack 1g -javamaxmem 16g " + modelFile + " -pf " + property;
    std::string result = execCommand(cmd);
    parseResult(result);
}

std::string PrismRunner::getPrismPath() {
    const char *prismPath = std::getenv("PRISM_PATH");
    if (!prismPath) {
        throw std::runtime_error("Environment variable PRISM_PATH is not set.");
    }
    // Check if the prism model checker exists and executable
    std::string prismBin = std::string(prismPath) + "/bin/prism";
    if (access(prismBin.c_str(), F_OK) == -1) {
        throw std::runtime_error("Could not find the prism model checker in PRISM_PATH.");
    }
    return prismBin;
}

std::string PrismRunner::execCommand(const std::string &command) {
    std::array<char, 128> buffer;
    std::string result;

    // Open a pipe to the command
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    // Read the command output
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

std::string PrismRunner::parseTime(const std::string &output) {
    std::string key = "Result:";
    auto pos = output.find(key);
    if (pos != std::string::npos) {
        size_t start = pos + key.length();
        size_t end = output.find_first_of("\r\n", start);
        return output.substr(start, end - start);
    }
    return "Result not found";
}

void PrismRunner::parseResult(const std::string &output) {
    std::string keys[] = {"Time for model checking:", "Result:" };
    for (auto key : keys) {
        auto pos = output.find(key);
        if (pos != std::string::npos) {
            // size_t start = pos + key.length();
            size_t start = pos;
            size_t end = output.find_first_of("\r\n", start);
            std::cout << output.substr(start, end - start) << std::endl;
        } else {
            std::cout << output << std::endl;
            std::cout << key << " not found" << std::endl;
        }
    }
}