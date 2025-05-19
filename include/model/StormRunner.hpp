//
// Created by CanhDo on 2025/05/19.
//

#ifndef STORMRUNNER_HPP
#define STORMRUNNER_HPP

#include "model/Runner.hpp"

class StormRunner : public Runner {
public:
    void modelCheck(const std::string &modelFile, const std::string &property) override;

    std::string getPath() override;

    bool isAvailable() override;

    void printTiming(const Timer &timer) override;
};

#endif //STORMRUNNER_HPP
