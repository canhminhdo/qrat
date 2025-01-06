//
// Created by CanhDo on 2024/12/09.
//

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <cstddef>
#include "utility/macros.hpp"

struct Configuration {
    // configuration options for the simulation scheme
    inline static double fidelityThreshold = 1e-8;
    inline static std::size_t seed = 0U;
    inline static bool outputBanner = true;
    inline static SystemMode systemMode = INTERACTIVE_MODE;
    inline static bool showTiming = true;
};

#endif //CONFIGURATION_HPP
