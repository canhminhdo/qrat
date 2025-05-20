//
// Created by CanhDo on 2025/05/19.
//

#include "model/Runner.hpp"
#include <array>
#include <memory>

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

void Runner::setSaveModel(bool saveModel) {
    this->saveModel = saveModel;
}

bool Runner::getSaveModel() {
    return saveModel;
}
