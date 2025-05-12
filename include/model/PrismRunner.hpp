//
// Created by Canh Minh Do on 2025/05/11.
//

#ifndef PRISMRUNNER_HPP
#define PRISMRUNNER_HPP

#include <string>

class PrismRunner {
public:
    static void modelCheck(const std::string &modelFile, const std::string &property);
    static std::string getPrismPath();

private:
    static std::string execCommand(const std::string &command);
    static std::string parseTime(const std::string &output);
    static void parseResult(const std::string &output);
};

#endif//PRISMRUNNER_HPP
