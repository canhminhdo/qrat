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
#include "ast/MeasExpNode.hpp"
#include "ast/UnitaryStmNode.hpp"
#include "dd/DDDefinitions.hpp"

using dd::Qubit;
using qc::StandardOperation;
using qc::Control;
using qc::Controls;
using qc::OP_NAME_TO_TYPE;

class DDSimulation {
public:
    DDSimulation(SyntaxProg *prog);

    qc::VectorDD getInitialState() const;

    qc::VectorDD generateRandomState();

    qc::VectorDD generateRandomState1();

    void initialize();

    void initQVarMap();

    void initQState();

    void initProperty();

    dd::GateMatrix getGateMatrix(UnitaryStmNode *stm);

    dd::TwoQubitGateMatrix getTwoQubitGateMatrix(UnitaryStmNode *stm);

    qc::VectorDD applyGate(UnitaryStmNode *stm, qc::VectorDD v);

    std::pair<qc::VectorDD, qc::VectorDD> measure(MeasExpNode *expr, qc::VectorDD v);

    qc::VectorDD project(qc::MatrixDD projector, qc::VectorDD v);

    qc::MatrixDD getProjector() const;

    bool test(qc::VectorDD v);

    bool test(qc::VectorDD v1, qc::VectorDD v2);

    void dump();

    void checkQubitRange(dd::Qubit qubit) {
        if (qubit > nqubits) {
            throw std::runtime_error("Qubit index out of range");
        }
    }


///---------------------------------------------------------------------------
///                            \n Operations \n
///---------------------------------------------------------------------------

#define DEFINE_SINGLE_TARGET_OPERATION(op)                                                  \
    StandardOperation op(const Qubit target) {                                              \
        return mc##op(Controls{}, target);                                                  \
    }                                                                                       \
    StandardOperation c##op(const Control& control, const Qubit target) {                   \
        return mc##op(Controls{control}, target);                                           \
    }                                                                                       \
    StandardOperation mc##op(const Controls& controls, const Qubit target) {                \
        checkQubitRange(target);                                                            \
        return StandardOperation(controls, target, OP_NAME_TO_TYPE.at(#op));                \
    }

    DEFINE_SINGLE_TARGET_OPERATION(i)
    DEFINE_SINGLE_TARGET_OPERATION(x)
    DEFINE_SINGLE_TARGET_OPERATION(y)
    DEFINE_SINGLE_TARGET_OPERATION(z)
    DEFINE_SINGLE_TARGET_OPERATION(h)
    DEFINE_SINGLE_TARGET_OPERATION(s)
    DEFINE_SINGLE_TARGET_OPERATION(sdg)
    DEFINE_SINGLE_TARGET_OPERATION(t)
    DEFINE_SINGLE_TARGET_OPERATION(tdg)
    DEFINE_SINGLE_TARGET_OPERATION(v)
    DEFINE_SINGLE_TARGET_OPERATION(vdg)
    DEFINE_SINGLE_TARGET_OPERATION(sx)
    DEFINE_SINGLE_TARGET_OPERATION(sxdg)

#undef DEFINE_SINGLE_TARGET_OPERATION

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
    // for properties
    qc::MatrixDD projector;

    // for random state generation
    Configuration config{};
    std::mt19937_64 mt{};
};
#endif //DDSIMULATION_HPP
