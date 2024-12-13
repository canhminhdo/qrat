//
// Created by CanhDo on 2024/11/21.
//

#ifndef DDSIMULATION_HPP
#define DDSIMULATION_HPP

#include "ir/QuantumComputation.hpp"
#include "dd/Package_fwd.hpp"
#include "DDSimulationPackageConfig.hpp"
#include "core/SymbolTable.hpp"
#include "core/SyntaxProg.hpp"
#include "core/VarSymbol.hpp"
#include "Configuration.hpp"

class DDSimulation {
public:
    DDSimulation(SyntaxProg *prog);

    qc::VectorDD generateRandomState();

    void setInitialState();

    void initialize();

    void initQVarMap();

    void initQState();

    void dump();

private:
    SyntaxProg *prog;
    using DDPackage = typename dd::Package<DDSimulationPackageConfig>;
    std::unique_ptr<DDPackage> dd;
    std::size_t nqubits{};
    qc::QuantumComputation *qc;

    using VectorDDMap = std::map<std::size_t, qc::VectorDD>;
    using QuantumVariableMap = std::map<int, std::size_t>;
    QuantumVariableMap qVarMap;
    VectorDDMap stateMap;
    qc::VectorDD initialState{};

    Configuration config{};
    std::mt19937_64 mt{};
};
#endif //DDSIMULATION_HPP
