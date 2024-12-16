//
// Created by CanhDo on 2024/11/21.
//

#include "dd/DDSimulation.hpp"

#include "core/Token.hpp"
#include "core/VarSymbol.hpp"
#include "dd/Package.hpp"
#include "dd/GateMatrixDefinitions.hpp"
#include "dd/Edge.hpp"

// using DDPackage = typename dd::Package<DDSimulationPackageConfig>;
DDSimulation::DDSimulation(SyntaxProg *prog) : prog{prog}, dd{std::make_unique<DDPackage>(prog->getNqubits())}, nqubits{prog->getNqubits()} {
    mt.seed(config.simulation.seed);
    initialize();
}

qc::VectorDD DDSimulation::generateRandomState() {

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

void DDSimulation::setInitialState() {
}

void DDSimulation::initialize() {
    initQVarMap();
    initQState();
}

void DDSimulation::initQVarMap() {
    std::vector<VarSymbol *> vars = prog->getVars();
    // sort variables by names
    sort(vars.begin(), vars.end(), [](VarSymbol *v1, VarSymbol *v2) {
        return std::strcmp(Token::name(v1->getName()), Token::name(v2->getName()));
    });
    for (int i = 0; i < vars.size(); i++) {
        qVarMap.insert({vars[i]->getName(), i});
    }
}

void DDSimulation::initQState() {
    std::vector<VarSymbol *> vars = prog->getVars();
    for (int i = 0; i < vars.size(); i++) {
        if (vars[i]->getValue()) {
            vars[i]->getValue()->dump();
        }
    }
}

void DDSimulation::dump() {
    generateRandomState();
    for (auto &qVar: qVarMap) {
        std::cout << Token::name(qVar.first) << " -> " << qVar.second << std::endl;
    }
}
