//
// Created by CanhDo on 2025/05/19.
//

#include "model/Runner.hpp"
#include "Configuration.hpp"

void Runner::printTiming(const Timer &timer) {
    if (Configuration::showTiming) {
        Int64 real;
        Int64 virt;
        Int64 prof;
        if (timer.getTimes(real, virt, prof)) {
            std::cout << "Time for model checking in " << prof / 1000 << "ms cpu (" << real / 1000 << "ms real)";
        }
    }
    std::cout << "\n";
}

std::string Runner::execCommand(const std::string &command) {
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

void Runner::parseResult(const std::string &output) {
    std::cout << "\n";
    std::string keys[] = {"Result"};
    for (auto key: keys) {
        auto pos = output.find(key);
        if (pos != std::string::npos) {
            // size_t start = pos + key.length();
            size_t start = pos;
            size_t end = output.find_first_of("\r\n", start);
            std::cout << output.substr(start, end - start) << std::endl;
        } else {
            std::cout << output << std::endl;
            std::cout << key << ": not found" << std::endl;
        }
    }
}
