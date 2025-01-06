//
// Created by CanhDo on 2024/12/09.
//

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <cstddef>

class Configuration {
public:
    // configuration options for the simulation scheme
    struct Simulation {
        static constexpr double fidelityThreshold = 1e-8;
        inline static std::size_t seed = 0U;
    };

    Simulation simulation{};
};

#endif //CONFIGURATION_HPP
