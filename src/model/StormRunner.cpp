//
// Created by CanhDo on 2025/05/19.
//

#include "model/StormRunner.hpp"
#include <iostream>
#include "utility/Tty.hpp"
#include <unistd.h>

void StormRunner::modelCheck(const std::string &modelFile, const std::string &property) {
    Timer timer(true);
    std::string stormBin = getPath();
    std::string cmd = stormBin + " --prism " + modelFile + " --prop " + property;
    std::string result = execCommand(cmd);
    parseResult(result);
    printTiming(timer);
}

std::string StormRunner::getPath() {
    const char *stormPath = std::getenv("STORM_PATH");
    return std::string(stormPath) + "/storm";
}

bool StormRunner::isAvailable() {
    const char *stormPath = std::getenv("STORM_PATH");
    if (stormPath == nullptr) {
        std::cerr << Tty(Tty::RED) << "Error: " << Tty(Tty::RESET) << "Environment variable STORM_PATH is not set." << std::endl;
        return false;
    }
    // Check if the Storm model checker exists and executable
    if (access(getPath().c_str(), F_OK) == -1) {
        std::cerr << Tty(Tty::RED) << "Error: " << Tty(Tty::RESET) << "Could not find the Storm model checker in STORM_PATH." << std::endl;
        return false;
    }
    return true;
}
