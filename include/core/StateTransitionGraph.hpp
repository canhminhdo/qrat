//
// Created by CanhDo on 2024/12/17.
//

#ifndef STATETRANSITIONGRAPH_HPP
#define STATETRANSITIONGRAPH_HPP

#include <unordered_set>
#include "utility/HashUtil.hpp"
#include "ast/StmNode.hpp"
#include "dd/DDSimulation.hpp"

class StateTransitionGraph {
public:
    struct State {
        StmNode *pc;
        qc::VectorDD current{};
        int parent;
        std::vector<size_t> nextStates;
        bool fullyExplored;
    };

    struct StateHash {
        std::size_t operator()(const State s) const {
            return HashUtil::combinedHash(s.pc->getHash(), std::hash<dd::vNode *>()(dynamic_cast<dd::vNode *>(s.current.p)));
        }
    };

    struct StateEqual {
        bool operator()(const State lhs, const State rhs) const {
            return lhs.pc == rhs.pc && lhs.current.p == rhs.current.p;
        }
    };

private:
    // for analysis
    std::unordered_set<State *, size_t, StateHash, StateEqual> stateTab;
    std::vector<State *> states;
    int idx;
};

#endif //STATETRANSITIONGRAPH_HPP
