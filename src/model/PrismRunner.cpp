//
// Created by Canh Minh Do on 2025/05/11.
//

#include "model/PrismRunner.hpp"
#include "Configuration.hpp"
#include <iostream>
#include <unistd.h>
#include "utility/Tty.hpp"

void PrismRunner::modelCheck(const std::string &modelFile, const std::string &property) {
    Timer timer(true);
    std::string prismBin = getPath();
    std::string cmd = prismBin + " -javastack 1g -javamaxmem 16g " + modelFile + " -pf " + property;
    std::string result = execCommand(cmd);
    parseResult(result);
    printTiming(timer);
}

std::string PrismRunner::getPath() {
    const char *prismPath = std::getenv("PRISM_PATH");
    return std::string(prismPath) + "/prism";
}

bool PrismRunner::isAvailable() {
    const char *prismPath = std::getenv("PRISM_PATH");
    if (prismPath == nullptr) {
        std::cerr << Tty(Tty::RED) << "Error: " << Tty(Tty::RESET) << "Environment variable PRISM_PATH is not set." <<
                std::endl;
        return false;
    }
    // Check if the prism model checker exists and executable
    if (access(getPath().c_str(), F_OK) == -1) {
        std::cerr << Tty(Tty::RED) << "Error: " << Tty(Tty::RESET) <<
                "Could not find the PRISM model checker in PRISM_PATH." << std::endl;
        return false;
    }
    return true;
}

void PrismRunner::printTiming(const Timer &timer) {
    if (Configuration::showTiming) {
        Int64 real;
        Int64 virt;
        Int64 prof;
        if (timer.getTimes(real, virt, prof)) {
            std::cout << "Time for model checking with PRISM in " << prof / 1000 << "ms cpu (" << real / 1000 <<
                    "ms real)";
        }
    }
    std::cout << "\n";
}
