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
#include "dd/DDDefinitions.hpp"

class DDSimulation {
public:
    DDSimulation(SyntaxProg *prog);

    qc::VectorDD getInitialState() const;

    qc::VectorDD generateRandomState();

    qc::VectorDD generateRandomState1();

    void initialize();

    void initQVarMap();

    void initQState();

    void dump();

private:
    // program
    SyntaxProg *prog;
    std::size_t nqubits{};

    // dd package
    using DDPackage = typename dd::Package<DDSimulationPackageConfig>;
    std::unique_ptr<DDPackage> dd;

    // mapping from variable to qubit and vice versa
    using QuantumVariableMap = std::map<int, std::size_t>;
    using RevQuantumVariableMap = std::map<int, std::size_t>;
    QuantumVariableMap qVarMap;
    RevQuantumVariableMap revQVarMap;

    // storing initial values
    using VectorDDMap = std::map<int, qc::VectorDD>;
    VectorDDMap initStateMap;
    qc::VectorDD initialState{};

    // for random state generation
    Configuration config{};
    std::mt19937_64 mt{};
};
#endif //DDSIMULATION_HPP
