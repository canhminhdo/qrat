//
// Created by CanhDo on 2026/11/14.
//

#ifndef CONTROLFLOWGRAPH_HPP
#define CONTROLFLOWGRAPH_HPP

#include <cstdint>
#include <vector>

class MeasExpNode;
class StmNode;
class StmSeq;
class UnitaryStmNode;

// Control flow for one parsed program, compiled once into `locations`.
// Each location is identified by its index in that vector; this index is the program counter in a search state.
// Successors are indices into the same vector. There is one location per statement occurrence in program order.
// If a statement node is shared across positions (see `StmNode::isShared()`), each position still gets its own location.
class ControlFlowGraph {
public:
    enum class Kind : std::uint8_t {
        SKIP,
        UNITARY,
        ATOMIC,
        BRANCH,
        HALT,
    };

    struct Location {
        Kind kind{Kind::HALT};
        // indices into locations. SKIP/UNITARY/ATOMIC use next[0]; BRANCH uses next[measurement outcome]
        int next[2]{0, 0};
        StmNode *stm{nullptr};               // source statement (for printing); null for HALT
        UnitaryStmNode *gate{nullptr};       // UNITARY
        MeasExpNode *meas{nullptr};          // BRANCH, for a conditional and a loop alike
        std::vector<UnitaryStmNode *> gates; // ATOMIC: the gates of the body in order, without skips
    };

    void build(const StmSeq *seq);

    int entry() const;

    int halt() const;

    const Location &at(int loc) const;

    int size() const;

    // for debugging
    void dump() const;

private:
    void numberLocations(const StmSeq *seq, std::vector<int> &endOf);

    int setNext(const StmSeq *seq, int first, int after, const std::vector<int> &endOf);

    std::vector<Location> locations;
};

#endif//CONTROLFLOWGRAPH_HPP
