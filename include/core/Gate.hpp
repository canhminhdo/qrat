//
// Created by CanhDo on 2024/11/20.
//

#ifndef GATE_HPP
#define GATE_HPP

#include <set>

enum class GateType {
    X,
    Y,
    Z,
    H,
    CX,
};

inline static const char *gateNames[] = {
    "X",
    "Y",
    "Z",
    "H",
    "CX",
};

inline static const std::set<GateType> gateArgs[] = {
    {},
    {GateType::X, GateType::Y, GateType::Z, GateType::H},
    {GateType::CX}
};

class Gate {
public:
    Gate(GateType type);

    const char *getName();

    int getArgsNr();

    GateType getType();

    void dump();

private:
    GateType type;
    int argsNr;
};
#endif //GATE_HPP
