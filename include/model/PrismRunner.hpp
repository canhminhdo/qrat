//
// Created by Canh Minh Do on 2025/05/11.
//

#ifndef PRISMRUNNER_HPP
#define PRISMRUNNER_HPP

#include <string>

#include "Runner.hpp"
#include "utility/Timer.hpp"

class PrismRunner : public Runner {
public:
    void modelCheck(const std::string &modelFile, const std::string &property) override;

    std::string getPath() override;

    bool isAvailable() override;

    void printTiming(const Timer &timer) override;
};

#endif//PRISMRUNNER_HPP
