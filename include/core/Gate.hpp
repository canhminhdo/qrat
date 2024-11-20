//
// Created by CanhDo on 2024/11/20.
//

#ifndef GATE_HPP
#define GATE_HPP

enum class GateType {
    X,
    Y,
    Z,
    H,
    CX,
};

class Gate {
public:
    Gate(GateType type);

    const char *getName();

    int getArgsNr();

private:
    GateType type;
    int argsNr;
};
#endif //GATE_HPP
