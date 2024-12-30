//
// Created by CanhDo on 2024/11/21.
//

#include "dd/DDSimulation.hpp"

#include "ast/BoolExpNode.hpp"
#include "ast/InitExpNode.hpp"
#include "ast/KetExpNode.hpp"
#include "ast/OpExpNode.hpp"
#include "ast/UnitaryStmNode.hpp"
#include "core/Token.hpp"
#include "core/VarSymbol.hpp"
#include "dd/Package.hpp"
#include "dd/GateMatrixDefinitions.hpp"
#include "dd/Edge.hpp"
#include "dd/Operations.hpp"

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

void DDSimulation::initProperty(ExpNode *expNode) {
    if (auto *propNode = dynamic_cast<PropExpNode *>(expNode)) {
        if (projectorMap.find(propNode) == projectorMap.end()) {
            projectorMap[propNode] = buildProjector(propNode);
        }
        return;
    }
    if (auto *opExpNode = dynamic_cast<OpExpNode *>(expNode)) {
        switch (opExpNode->getType()) {
            case OpExpType::NOT:
                initProperty(opExpNode->getRight());
                break;
            case OpExpType::AND:
            case OpExpType::OR:
                initProperty(opExpNode->getLeft());
                initProperty(opExpNode->getRight());
                break;
            default:
                throw std::runtime_error("Unsupported property type");
        }
    }
}

qc::Controls DDSimulation::buildControls(UnitaryStmNode *stm) {
    qc::Controls controls;
    for (int i = 0; i < stm->getControls().size(); i++) {
        auto cQubit = qVarMap[stm->getControls().at(i)->getName()];
        controls.insert({cQubit});
    }
    return controls;
}

qc::Targets DDSimulation::buildTargets(UnitaryStmNode *stm) {
    qc::Targets targets;
    for (int i = 0; i < stm->getTargets().size(); i++) {
        auto tQubit = getQubit(stm->getTargets().at(i));
        targets.push_back(tQubit);
    }
    return targets;
}

qc::Qubit DDSimulation::getQubit(Symbol *symbol) {
    assert(dynamic_cast<VarSymbol *>(symbol) != nullptr);
    return qVarMap[symbol->getName()];
}

qc::MatrixDD DDSimulation::buildProjector(PropExpNode *propNode) {
    if (propNode->getVars().size() != 1) {
        throw std::runtime_error("Only support property with one variable");
    }
    auto target = qVarMap[propNode->getVars()[0]->getName()];
    if (auto *ketNode = dynamic_cast<KetExpNode *>(propNode->getExpr())) {
        if (ketNode->getType() == KetType::KET_ZERO) {
            auto v0 = dd->makeBasisState(1, std::vector<bool>{false});
            return dd->outerProduct(v0, target);
        }
        if (ketNode->getType() == KetType::KET_ONE) {
            auto v1 = dd->makeBasisState(1, std::vector<bool>{true});
            return dd->outerProduct(v1, target);
        }
        throw std::runtime_error("Only support initialization with |0>, |1> or initial state");
    }
    if (auto *initNode = dynamic_cast<InitExpNode *>(propNode->getExpr())) {
        return dd->outerProduct(initStateMap[initNode->getVar()->getName()], target);
    }
    throw std::runtime_error("Only support projector from |0>, |1> or initial state");
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

qc::VectorDD DDSimulation::applyGate(UnitaryStmNode *stm, qc::VectorDD v) {
    auto controls = buildControls(stm);
    qc::Targets targets = buildTargets(stm);
    auto params = stm->getParams();
    qc::StandardOperation op = StandardOperation(controls, targets, stm->getOpType(), params);
    auto gate = dd::getDD<DDSimulationPackageConfig>(&op, *dd);
    auto v1 = dd->multiply(gate, v);
    return v1;
    // if (targets.size() == 1) {
    //     auto gate = dd::getStandardOperationDD<DDSimulationPackageConfig>(&op, *dd, controls, targets.front(), false);
    //     auto v1 = dd->multiply(gate, v);
    //     return v1;
    // }
    // if (targets.size() == 2) {
    //     auto gate = dd::getStandardOperationDD<DDSimulationPackageConfig>(&op, *dd, controls, targets.front(), targets.back(), false);
    //     auto v1 = dd->multiply(gate, v);
    //     return v1;
    // }
    // throw std::runtime_error("Only support 1 & 2 qubit gate");
}

std::pair<qc::VectorDD, qc::VectorDD> DDSimulation::measure(MeasExpNode *expr, qc::VectorDD v) {
    auto var = expr->getVar();
    auto target = qVarMap[var->getName()];
    auto v0 = dd->measureOneQubit(v, target, true);
    auto v1 = dd->measureOneQubit(v, target, false);
    return {v0, v1};
}

std::tuple<qc::VectorDD, qc::fp, qc::VectorDD, qc::fp>
DDSimulation::measureWithProb(MeasExpNode *expr, qc::VectorDD v) {
    auto var = expr->getVar();
    auto target = qVarMap[var->getName()];
    return dd->measureOneQubit(v, target);
}

qc::VectorDD DDSimulation::project(qc::MatrixDD projector, qc::VectorDD v) {
    return dd->multiply(projector, v);
}

bool DDSimulation::test(qc::VectorDD v, ExpNode *expNode) {
    if (auto *boolExp = dynamic_cast<BoolExpNode *>(expNode)) {
        if (boolExp->getVal() == BoolType::TRUE) {
            return true;
        } else if (boolExp->getVal() == BoolType::FALSE) {
            return false;
        } else {
            throw std::runtime_error("Unsupported boolean expression");
        }
    }
    if (auto *propNode = dynamic_cast<PropExpNode *>(expNode)) {
        auto projector = projectorMap[propNode];
        auto v1 = dd->multiply(projector, v);
        if (v.p == v1.p) {
            return true;
        }
        return false;
        // todo: should check structure similarity during checking fidelity for fast comparison
        // auto fd = dd->fidelity(v1, v);
        // std::cout << "Fidelity: " << fd << std::endl;
        // return std::abs(fd - 1) < config.simulation.fidelityThreshold;
    }
    if (auto *opExpNode = dynamic_cast<OpExpNode *>(expNode)) {
        switch (opExpNode->getType()) {
            case OpExpType::NOT:
                return not test(v, opExpNode->getRight());
            case OpExpType::AND:
                if (test(v, opExpNode->getLeft())) {
                    return test(v, opExpNode->getRight());
                }
                return false;
            case OpExpType::OR:
                if (test(v, opExpNode->getLeft())) {
                    return true;
                }
                return test(v, opExpNode->getRight());
            default:
                throw std::runtime_error("Unsupported property type");
        }
    }
    throw std::runtime_error("Unsupported expression type");
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
    std::cout << "Projectors " << std::endl;
    for (auto &projector: projectorMap) {
        std::cout << "Property: " << std::endl;
        projector.first->dump(true);
        projector.second.printMatrix<dd::mNode>(nqubits);
    }
}
