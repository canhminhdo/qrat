//
// Created by CanhDo on 2024/11/21.
//

#include "dd/DDSimulation.hpp"

#include "ast/KetExpNode.hpp"
#include "ast/UnitaryStmNode.hpp"
#include "core/Token.hpp"
#include "core/VarSymbol.hpp"
#include "dd/Package.hpp"
#include "dd/GateMatrixDefinitions.hpp"
#include "dd/Edge.hpp"

// using DDPackage = typename dd::Package<DDSimulationPackageConfig>;
DDSimulation::DDSimulation(SyntaxProg *prog) : prog{prog}, dd{std::make_unique<DDPackage>(prog->getNqubits())},
                                               nqubits{prog->getNqubits()} {
    mt.seed(config.simulation.seed);
    initialize();
}

qc::VectorDD DDSimulation::getInitialState() const {
    return initialState;
}

qc::VectorDD DDSimulation::generateRandomState() {
    // Uniform distribution for theta in [0, pi]
    std::uniform_real_distribution<dd::fp> dist_theta(0.0, dd::PI_4);
    // Uniform distribution for phi in [0, 2*pi]
    std::uniform_real_distribution<dd::fp> dist_phi(0.0, 2 * dd::PI_4);
    // Generate random theta and phi
    dd::fp theta = dist_theta(mt);
    dd::fp phi = dist_phi(mt);

    // Apply R_y and R_z gates to make a random state
    auto v = dd->makeBasisState(1, std::vector<bool>{false});
    auto gateRY = dd->makeGateDD(dd::ryMat(theta), 0);
    auto gateRZ = dd->makeGateDD(dd::rzMat(phi), 0);
    auto v1 = dd->multiply(gateRY, v);
    auto v2 = dd->multiply(gateRZ, v1);
    return v2;
}

qc::VectorDD DDSimulation::generateRandomState1() {
    // Uniform distribution for theta in [0, pi]
    std::uniform_real_distribution<dd::fp> dist_theta(0.0, dd::PI_4);
    // Uniform distribution for phi in [0, 2*pi]
    std::uniform_real_distribution<dd::fp> dist_phi(0.0, 2 * dd::PI_4);
    // Generate random theta and phi
    dd::fp theta = dist_theta(mt);
    dd::fp phi = dist_phi(mt);
    std::cout << "theta: " << theta << ", phi: " << phi << std::endl;

    // for properties
    std::vector<bool> stimulusBitsZero(1, false);
    std::vector<bool> stimulusBitsOne(1, true);
    auto vOne = dd->makeBasisState(1, stimulusBitsOne);
    vOne.printVector<dd::vNode>();
    auto outerProd = dd->outerProduct(vOne, 0);
    auto vOne1 = dd->multiply(outerProd, vOne);
    vOne1.printVector<dd::vNode>();
    // auto gateRY12 = dd->makeGateDD(dd::ryMat(theta), 0);
    // auto vOne1 = dd->multiply(gateRY12, vOne);
    // vOne1.printVector<dd::vNode>();
    auto vZero = dd->makeBasisState(1, stimulusBitsZero);
    auto outerProd1 = dd->outerProduct(vZero, 0);
    auto vZero1 = dd->multiply(outerProd1, vOne);
    vZero1.printVector<dd::vNode>();

    std::vector<bool> stimulusBits(nqubits, false);
    auto v = dd->makeBasisState(nqubits, stimulusBits);
    // Apply R_y and R_z gates to make a random state
    auto gateRY = dd->makeGateDD(dd::ryMat(theta), 0);
    auto gateRZ = dd->makeGateDD(dd::rzMat(phi), 0);
    auto v00 = dd->multiply(gateRY, v);
    auto v0 = dd->multiply(gateRZ, v00);

    // tensor product
    auto q0 = dd->makeBasisState(1, std::vector<bool>{false});
    auto q1 = dd->makeBasisState(1, std::vector<bool>{false});
    auto q2 = dd->makeBasisState(1, std::vector<bool>{false});
    auto q01 = dd->multiply(gateRY, q0);
    auto q02 = dd->multiply(gateRZ, q01);
    auto v12 = dd->kronecker(q2, q1, 1);
    auto v123 = dd->kronecker(v12, q02, 1);
    std::cout << "----- q01 -----" << std::endl;
    q01.printVector<dd::vNode>();
    std::cout << "----- q02 -----" << std::endl;
    q02.printVector<dd::vNode>();
    std::cout << "----- v12 -----" << std::endl;
    v12.printVector<dd::vNode>();
    std::cout << "----- v123 -----" << std::endl;
    v123.printVector<dd::vNode>();


    //    auto gateH0 = dd->makeGateDD(dd::H_MAT, 0);
    //    auto v0 = dd->multiply(gateH0, v);
    auto gateH1 = dd->makeGateDD(dd::H_MAT, 1);
    auto v1 = dd->multiply(gateH1, v0);
    auto gateCX1 = dd->makeTwoQubitGateDD(dd::CX_MAT, 1, 2);
    auto v2 = dd->multiply(gateCX1, v1);
    auto gateCX2 = dd->makeTwoQubitGateDD(dd::CX_MAT, 0, 1);
    auto v3 = dd->multiply(gateCX2, v2);
    auto gateH2 = dd->makeGateDD(dd::H_MAT, 0);
    auto v4 = dd->multiply(gateH2, v3);

    // 00
    auto v5_0 = dd->measureOneQubit(v4, 1, true);
    auto v6_0 = dd->measureOneQubit(v5_0, 0, true);

    auto testGate = dd->outerProduct(q02, 2);
    auto vTest = dd->multiply(testGate, v6_0);
    std::cout << "----- vTest -----" << std::endl;
    vTest.printVector<dd::vNode>();
    auto fd = dd->fidelity(vTest, v6_0);

    // 01
    // auto v5_0 = dd->measureOneQubit(v4, 1, true);
    // auto v6_1 = dd->measureOneQubit(v5_0, 0, false);
    // auto gateZ = dd->makeGateDD(dd::Z_MAT, 2);
    // auto v7 = dd->multiply(gateZ, v6_1);

    // 10
    // auto v5_1 = dd->measureOneQubit(v4, 1, false);
    // auto gateX = dd->makeGateDD(dd::X_MAT, 2);
    // auto v6 = dd->multiply(gateX, v5_1);
    // auto v7_0 = dd->measureOneQubit(v6, 0, true);

    // 11
    // auto v5_1 = dd->measureOneQubit(v4, 1, false);
    // auto gateX = dd->makeGateDD(dd::X_MAT, 2);
    // auto v6 = dd->multiply(gateX, v5_1);
    // auto v7_1 = dd->measureOneQubit(v6, 0, false);
    // auto gateZ = dd->makeGateDD(dd::Z_MAT, 2);
    // auto v8 = dd->multiply(gateZ, v7_1);

    std::cout << "----- v0 -----" << std::endl;
    v0.printVector<dd::vNode>();
    std::cout << "----- v1 -----" << std::endl;
    v1.printVector<dd::vNode>();
    std::cout << "----- v2 -----" << std::endl;
    v2.printVector<dd::vNode>();
    std::cout << "----- v3 -----" << std::endl;
    v3.printVector<dd::vNode>();
    std::cout << "----- v4 -----" << std::endl;
    v4.printVector<dd::vNode>();

    // 00
    std::cout << "----- v5_0 -----" << std::endl;
    v5_0.printVector<dd::vNode>();
    std::cout << "----- v6_0 -----" << std::endl;
    v6_0.printVector<dd::vNode>();
    std::cout << "----- vTest -----" << std::endl;
    vTest.printVector<dd::vNode>();
    std::cout << "fidelity = " << fd << std::endl;


    // 01
    // std::cout << "----- v5_0 -----" << std::endl;
    // v5_0.printVector<dd::vNode>();
    // std::cout << "----- v6_1 -----" << std::endl;
    // v6_1.printVector<dd::vNode>();
    // std::cout << "----- v7 -----" << std::endl;
    // v7.printVector<dd::vNode>();

    // 10
    // std::cout << "----- v5_1 -----" << std::endl;
    // v5_1.printVector<dd::vNode>();
    // std::cout << "----- v6 -----" << std::endl;
    // v6.printVector<dd::vNode>();
    // std::cout << "----- v7_0 -----" << std::endl;
    // v7_0.printVector<dd::vNode>();

    // 11
    // std::cout << "----- v5_1 -----" << std::endl;
    // v5_1.printVector<dd::vNode>();
    // std::cout << "----- v6 -----" << std::endl;
    // v6.printVector<dd::vNode>();
    // std::cout << "----- v7_1 -----" << std::endl;
    // v7_1.printVector<dd::vNode>();
    // std::cout << "----- v8 -----" << std::endl;
    // v8.printVector<dd::vNode>();

    return v0;
}

void DDSimulation::initialize() {
    initQVarMap();
    initQState();
}

void DDSimulation::initQVarMap() {
    std::vector<VarSymbol *> vars = prog->getVars();
    // lexically sort variables by names in lexicographical order
    sort(vars.begin(), vars.end(), [](VarSymbol *v1, VarSymbol *v2) {
        return std::strcmp(Token::name(v1->getName()), Token::name(v2->getName())) < 0;
    });
    for (int i = 0; i < vars.size(); i++) {
        qVarMap.insert({vars[i]->getName(), i});
        revQVarMap.insert({i, vars[i]->getName()});
    }
}

void DDSimulation::initQState() {
    std::vector<VarSymbol *> vars = prog->getVars();
    for (int i = 0; i < vars.size(); i++) {
        if (Node *node = vars[i]->getValue(); node != nullptr) {
            if (KetExpNode *ketNode = dynamic_cast<KetExpNode *>(node); ketNode != nullptr) {
                switch (ketNode->getType()) {
                    case KetType::KET_ZERO:
                        initStateMap[vars[i]->getName()] = dd->makeBasisState(1, std::vector<bool>{false});
                        break;
                    case KetType::KET_ONE:
                        initStateMap[vars[i]->getName()] = dd->makeBasisState(1, std::vector<bool>{true});
                        break;
                    case KetType::KET_RANDOM:
                        initStateMap[vars[i]->getName()] = generateRandomState();
                        break;
                    default:
                        throw std::runtime_error("Only support initialization with |0>, |1> or random state");
                }
                dd->incRef(initStateMap[vars[i]->getName()]);
            }
        } else {
            // not initialized, then set to |0> as default
            initStateMap[vars[i]->getName()] = dd->makeBasisState(1, std::vector<bool>{false});
        }
    }
    assert(vars.size() > 0);
    // building initial state
    initialState = initStateMap[vars[0]->getName()];
    for (int i = 1; i < vars.size(); i++) {
        initialState = dd->kronecker(initStateMap[vars[i]->getName()], initialState, i);
    }
    dd->incRef(initialState);
}

void DDSimulation::initProperty() {
    projector = dd->outerProduct(initStateMap[0], 2);
}

qc::VectorDD DDSimulation::applyGate(StmNode *stm, qc::VectorDD v) {
    auto unitaryStm = dynamic_cast<UnitaryStmNode *>(stm);
    assert(unitaryStm != nullptr);
    auto vars = unitaryStm->getVars();
    auto nArgs = vars.size();
    if (nArgs == 1) {
        auto target = qVarMap[vars[0]->getName()];
        auto gateMat = getGateMatrix(unitaryStm);
        auto gate = dd->makeGateDD(gateMat, target);
        auto v1 = dd->multiply(gate, v);
        return v1;
    }
    if (nArgs == 2) {
        auto target1 = qVarMap[vars[0]->getName()];
        auto target2 = qVarMap[vars[1]->getName()];
        auto gateTwoQubitMat = getTwoQubitGateMatrix(unitaryStm);
        auto gateTwoQubit = dd->makeTwoQubitGateDD(gateTwoQubitMat, target1, target2);
        auto v2 = dd->multiply(gateTwoQubit, v);
        return v2;
    }
    throw std::runtime_error("Only support 1 or 2 qubit gate");
}

dd::GateMatrix DDSimulation::getGateMatrix(UnitaryStmNode *stm) {
    auto vars = stm->getVars();
    assert(vars.size() == 1);
    switch (stm->getGateExp()->getGate()->getType()) {
        case GateType::X:
            return dd::X_MAT;
        case GateType::Y:
            return dd::Y_MAT;
        case GateType::Z:
            return dd::Z_MAT;
        case GateType::H:
            return dd::H_MAT;
        default:
            throw std::runtime_error("Unsupported gate type");
    }
}

dd::TwoQubitGateMatrix DDSimulation::getTwoQubitGateMatrix(UnitaryStmNode *stm) {
    auto vars = stm->getVars();
    assert(vars.size() == 2);
    switch (stm->getGateExp()->getGate()->getType()) {
        case GateType::CX:
            return dd::CX_MAT;
        case GateType::CZ:
            return dd::CZ_MAT;
        default:
            throw std::runtime_error("Unsupported gate type");
    }
}

qc::MatrixDD DDSimulation::getProjector() const {
    return projector;
}

bool DDSimulation::test(qc::VectorDD v) {
    auto v1 = dd->multiply(projector, v);
    if (v.p == v1.p) {
        return true;
    }
    auto fd = dd->fidelity(v1, v);
    return std::abs(fd - 1) > config.simulation.fidelityThreshold;
}

bool DDSimulation::test(qc::VectorDD v1, qc::VectorDD v2) {
    return v1.p == v2.p;
}

void DDSimulation::dump() {
    for (auto &qVar: qVarMap) {
        std::cout << Token::name(qVar.first) << " -> " << qVar.second << std::endl;
    }
    for (auto &refQVar: revQVarMap) {
        std::cout << refQVar.first << " -> " << Token::name(refQVar.second) << std::endl;
    }
    for (auto &qVarVal: initStateMap) {
        std::cout << Token::name(qVarVal.first) << " -> " << std::endl;
        qVarVal.second.printVector<dd::vNode>();
    }
    std::cout << "Initial state: " << std::endl;
    initialState.printVector<dd::vNode>();
}
