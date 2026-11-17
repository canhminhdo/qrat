//
// Created by CanhDo on 2026/11/14.
//

#include "core/ControlFlowGraph.hpp"
#include "ast/AtomicStmNode.hpp"
#include "ast/CondExpNode.hpp"
#include "ast/CondStmNode.hpp"
#include "ast/MeasExpNode.hpp"
#include "ast/NumExpNode.hpp"
#include "ast/SkipStmNode.hpp"
#include "ast/StmSeq.hpp"
#include "ast/UnitaryStmNode.hpp"
#include "ast/WhileStmNode.hpp"
#include <iostream>
#include <stdexcept>
#include <utility>

namespace {
    // Splits a guard M[q] = b into its measurement and the outcome b that satisfies it.
    std::pair<MeasExpNode *, int> guardOf(ExpNode *cond) {
        auto *condExp = dynamic_cast<CondExpNode *>(cond);
        auto *measExp = condExp != nullptr ? dynamic_cast<MeasExpNode *>(condExp->getLeft()) : nullptr;
        auto *numExp = condExp != nullptr ? dynamic_cast<NumExpNode *>(condExp->getRight()) : nullptr;
        if (measExp == nullptr || numExp == nullptr) {
            throw std::runtime_error("Unsupported guard: expected M[q] = 0 or M[q] = 1");
        }
        return {measExp, numExp->isZero() ? 0 : 1};
    }

    const char *kindName(ControlFlowGraph::Kind kind) {
        switch (kind) {
            case ControlFlowGraph::Kind::SKIP:
                return "SKIP";
            case ControlFlowGraph::Kind::UNITARY:
                return "UNITARY";
            case ControlFlowGraph::Kind::ATOMIC:
                return "ATOMIC";
            case ControlFlowGraph::Kind::BRANCH:
                return "BRANCH";
            case ControlFlowGraph::Kind::HALT:
                return "HALT";
        }
        return "?";
    }
}// namespace

void ControlFlowGraph::build(const StmSeq *seq) {
    locations.clear();
    // endOf[i] is the index one past the last location of the statement at location i, counting the
    // statements nested in it; only needed while building
    std::vector<int> endOf;
    // pass 1: one location per statement in source order, then HALT as the last one
    numberLocations(seq, endOf);
    locations.emplace_back();
    // pass 2: the program starts at entry() and is followed by HALT
    setNext(seq, entry(), halt(), endOf);
}

int ControlFlowGraph::entry() const {
    return 0;
}

int ControlFlowGraph::halt() const {
    return static_cast<int>(locations.size()) - 1;
}

const ControlFlowGraph::Location &ControlFlowGraph::at(int loc) const {
    return locations.at(loc);
}

int ControlFlowGraph::size() const {
    return static_cast<int>(locations.size());
}

// Pass 1. Appends one location for every statement of the list, and of the lists nested in it, in source
// pre-order, and records the kind and the payload. Because a location is appended when its statement is
// visited, the statements of a list sit at increasing indices, each followed by the locations nested in it.
// A statement is identified by this position, never by its node: a shared node occurs at several positions.
// Statements inside an atomic body get no location: the body is executed as a single transition, so no
// program counter ever points into it.
void ControlFlowGraph::numberLocations(const StmSeq *seq, std::vector<int> &endOf) {
    if (seq == nullptr) {
        return;
    }
    for (auto *stm: seq->getStms()) {
        const int index = static_cast<int>(locations.size());
        locations.emplace_back();
        endOf.push_back(0);
        locations[index].stm = stm;
        if (dynamic_cast<SkipStmNode *>(stm) != nullptr) {
            locations[index].kind = Kind::SKIP;
        } else if (auto *unitaryStm = dynamic_cast<UnitaryStmNode *>(stm); unitaryStm != nullptr) {
            locations[index].kind = Kind::UNITARY;
            locations[index].gate = unitaryStm;
        } else if (auto *condStm = dynamic_cast<CondStmNode *>(stm); condStm != nullptr) {
            locations[index].kind = Kind::BRANCH;
            locations[index].meas = guardOf(condStm->getCond()).first;
            numberLocations(condStm->getThenStm(), endOf);
            numberLocations(condStm->getElseStm(), endOf);
        } else if (auto *whileStm = dynamic_cast<WhileStmNode *>(stm); whileStm != nullptr) {
            locations[index].kind = Kind::BRANCH;
            locations[index].meas = guardOf(whileStm->getCond()).first;
            numberLocations(whileStm->getBody(), endOf);
        } else if (auto *atomicStm = dynamic_cast<AtomicStmNode *>(stm); atomicStm != nullptr) {
            locations[index].kind = Kind::ATOMIC;
            for (auto *inner: atomicStm->getBody()->getStms()) {
                if (auto *gate = dynamic_cast<UnitaryStmNode *>(inner); gate != nullptr) {
                    locations[index].gates.push_back(gate);
                } else if (dynamic_cast<SkipStmNode *>(inner) == nullptr) {
                    throw std::runtime_error("Unknown statement type in atomic statement");
                }
            }
        } else {
            throw std::runtime_error("Unknown statement type");
        }
        endOf[index] = static_cast<int>(locations.size());
    }
}

// Pass 2. Fills in next[] for every location of the list whose first statement sits at `first`, and returns
// the index one past the list. A statement is followed by the next statement of its list, and the last
// statement by `after`, the location that runs once the whole list has finished.
// Executing the list L from its first statement runs its statements in order and then continues at `after`.
//   - A statement that is not the last one is followed by the next statement of L, which is where f points.
//   - The last statement is followed by `after`.
//   - A conditional runs one branch and is then finished, so each branch is followed by the f of the
//     conditional; by the induction hypothesis the branch continues there once it has run.
//   - A loop tests its guard again after the body, so the body is followed by the loop itself, and the loop is
//     left through f when the guard fails.
// The outcome b satisfies the guard, so next[b] enters the then-branch or the body.
int ControlFlowGraph::setNext(const StmSeq *seq, int first, int after, const std::vector<int> &endOf) {
    int index = first;
    if (seq == nullptr) {
        return index;
    }
    const auto &stms = seq->getStms();
    for (std::size_t i = 0; i < stms.size(); i++) {
        auto *stm = stms[i];
        const int end = endOf[index];
        const int f = i + 1 < stms.size() ? end : after;
        if (auto *condStm = dynamic_cast<CondStmNode *>(stm); condStm != nullptr) {
            const int b = guardOf(condStm->getCond()).second;
            const int thenFirst = index + 1;
            const int elseFirst = setNext(condStm->getThenStm(), thenFirst, f, endOf);
            setNext(condStm->getElseStm(), elseFirst, f, endOf);
            locations[index].next[b] = condStm->getThenStm()->empty() ? f : thenFirst;
            locations[index].next[1 - b] = condStm->getElseStm()->empty() ? f : elseFirst;
        } else if (auto *whileStm = dynamic_cast<WhileStmNode *>(stm); whileStm != nullptr) {
            const int b = guardOf(whileStm->getCond()).second;
            locations[index].next[b] = whileStm->getBody()->empty() ? index : index + 1;
            locations[index].next[1 - b] = f;
            setNext(whileStm->getBody(), index + 1, index, endOf);
        } else {
            locations[index].next[0] = f;
        }
        index = end;
    }
    return index;
}

void ControlFlowGraph::dump() const {
    for (int i = 0; i < size(); i++) {
        const auto &loc = locations[i];
        std::cout << i << ": " << kindName(loc.kind);
        if (loc.kind == Kind::BRANCH) {
            std::cout << " next[0]=" << loc.next[0] << " next[1]=" << loc.next[1];
        } else if (loc.kind != Kind::HALT) {
            std::cout << " next=" << loc.next[0];
        }
        std::cout << std::endl;
    }
}
