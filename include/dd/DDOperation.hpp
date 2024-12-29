//
// Created by CanhDo on 2024/12/27.
//

#ifndef DDOPERATION_HPP
#define DDOPERATION_HPP

#include "ast/UnitaryStmNode.hpp"
#include "core/SyntaxProg.hpp"
#include "ir/operations/StandardOperation.hpp"

enum OperationType {
    SINGLE_TARGET_OP,
    SINGLE_TARGET_OP_WITH_ONE_PARAM,
    SINGLE_TARGET_OP_WITH_TWO_PARAM,
    SINGLE_TARGET_OP_WITH_THREE_PARAM,
    SINGLE_TARGET_COP,
    SINGLE_TARGET_COP_WITH_ONE_PARAM,
    SINGLE_TARGET_COP_WITH_TWO_PARAM,
    SINGLE_TARGET_COP_WITH_THREE_PARAM,
    SINGLE_TARGET_MCOP,
    SINGLE_TARGET_MCOP_WITH_ONE_PARAM,
    SINGLE_TARGET_MCOP_WITH_TWO_PARAM,
    SINGLE_TARGET_MCOP_WITH_THREE_PARAM,
    TWO_TARGET_OP,
    TWO_TARGET_OP_WITH_ONE_PARAM,
    TWO_TARGET_OP_WITH_TWO_PARAM,
    TWO_TARGET_COP,
    TWO_TARGET_COP_WITH_ONE_PARAM,
    TWO_TARGET_COP_WITH_TWO_PARAM,
    TWO_TARGET_MCOP,
    TWO_TARGET_MCOP_WITH_ONE_PARAM,
    TWO_TARGET_MCOP_WITH_TWO_PARAM
};

struct GateInfo {
    Token token;
    OperationType type;
};

class DDOperation {
public:
    inline static UnitaryStmNode *makeOperation(SyntaxProg *prog, TokenList *vars1, GateInfo gateInfo, TokenList *vars2, std::vector<qc::fp> *params = nullptr) {
        std::string opName(gateInfo.token.name());
        std::transform(opName.begin(), opName.end(), opName.begin(), [](unsigned char c) { return std::tolower(c); });
        qc::OpType type = qc::opTypeFromString(opName);
        validateArgs(vars1, vars2);
        auto varSymbols = validateSymbols(prog, vars1);
        UnitaryStmNode *unitaryStmNode;
        auto parameters = params == nullptr ? std::vector<qc::fp>{} : *params;
        switch (gateInfo.type) {
            case SINGLE_TARGET_OP_WITH_ONE_PARAM:
                validateOneParams(params);
                goto SINGLE_TARGET_OP;
            case SINGLE_TARGET_OP_WITH_TWO_PARAM:
                validateTwoParams(params);
                goto SINGLE_TARGET_OP;
            case SINGLE_TARGET_OP_WITH_THREE_PARAM:
                validateThreeParams(params);
                goto SINGLE_TARGET_OP;
            case SINGLE_TARGET_OP: {
                SINGLE_TARGET_OP:
                validateOneArgs(vars1);
                unitaryStmNode = new UnitaryStmNode(gateInfo.token.code(), type, {}, varSymbols, parameters);
                break;
            }
            case SINGLE_TARGET_COP_WITH_ONE_PARAM:
                validateOneParams(params);
                goto SINGLE_TARGET_COP;
            case SINGLE_TARGET_COP_WITH_TWO_PARAM:
                validateTwoParams(params);
                goto SINGLE_TARGET_COP;
            case SINGLE_TARGET_COP_WITH_THREE_PARAM:
                validateThreeParams(params);
                goto SINGLE_TARGET_COP;
            case SINGLE_TARGET_COP: {
                SINGLE_TARGET_COP:
                validateTwoArgs(vars1);
                unitaryStmNode = new UnitaryStmNode(gateInfo.token.code(), type, {varSymbols.front()},
                                                    {varSymbols.back()}, parameters);
                break;
            }
            case SINGLE_TARGET_MCOP_WITH_ONE_PARAM:
                validateOneParams(params);
                goto SINGLE_TARGET_MCOP;
            case SINGLE_TARGET_MCOP_WITH_TWO_PARAM:
                validateTwoParams(params);
                goto SINGLE_TARGET_MCOP;
            case SINGLE_TARGET_MCOP_WITH_THREE_PARAM:
                validateThreeParams(params);
                goto SINGLE_TARGET_MCOP;
            case SINGLE_TARGET_MCOP: {
                SINGLE_TARGET_MCOP:
                validateTwoOrMoreArgs(vars1);
                std::vector<Symbol *> controls(varSymbols.begin(), varSymbols.end() - 1);
                unitaryStmNode = new UnitaryStmNode(gateInfo.token.code(), type, controls, {varSymbols.back()}, parameters);
                break;
            }
            case TWO_TARGET_OP_WITH_ONE_PARAM:
                validateOneParams(params);
                goto TWO_TARGET_OP;
            case TWO_TARGET_OP_WITH_TWO_PARAM:
                validateTwoParams(params);
                goto TWO_TARGET_OP;
            case TWO_TARGET_OP: {
                TWO_TARGET_OP:
                validateTwoArgs(vars1);
                unitaryStmNode = new UnitaryStmNode(gateInfo.token.code(), type, {},
                                                    varSymbols, parameters);
                break;
            }
            case TWO_TARGET_COP_WITH_ONE_PARAM:
                validateOneParams(params);
                goto TWO_TARGET_COP;
            case TWO_TARGET_COP_WITH_TWO_PARAM:
                validateTwoParams(params);
                goto TWO_TARGET_COP;
            case TWO_TARGET_COP: {
                TWO_TARGET_COP:
                validateThreeArgs(vars1);
                std::vector<Symbol *> targets(varSymbols.end() - 2, varSymbols.end());
                unitaryStmNode = new UnitaryStmNode(gateInfo.token.code(), type, {varSymbols.front()}, targets, parameters);
                break;
            }
            case TWO_TARGET_MCOP_WITH_ONE_PARAM:
                validateOneParams(params);
                goto TWO_TARGET_MCOP;
            case TWO_TARGET_MCOP_WITH_TWO_PARAM:
                validateTwoParams(params);
                goto TWO_TARGET_MCOP;
            case TWO_TARGET_MCOP: {
                TWO_TARGET_MCOP:
                validateThreeOrMoreArgs(vars1);
                std::vector<Symbol *> controls(varSymbols.begin(), varSymbols.end() - 2);
                std::vector<Symbol *> targets(varSymbols.end() - 2, varSymbols.end());
                unitaryStmNode = new UnitaryStmNode(gateInfo.token.code(), type, controls, targets, parameters);
                break;
            }
        }
        return unitaryStmNode;
    }

    inline static void validateOneParams(std::vector<qc::fp> *params) {
        if (params == nullptr || params->size() != 1) {
            throw std::runtime_error("Invalid number of parameters for the operation");
        }
    }

    inline static void validateTwoParams(std::vector<qc::fp> *params) {
        if (params == nullptr || params->size() != 2) {
            throw std::runtime_error("Invalid number of parameters for the operation");
        }
    }

    inline static void validateThreeParams(std::vector<qc::fp> *params) {
        if (params == nullptr || params->size() != 3) {
            throw std::runtime_error("Invalid number of parameters for the operation");
        }
    }

    inline static void validateArgs(TokenList *vars1, TokenList *vars2) {
        if (vars1->size() != vars2->size()) {
            throw std::runtime_error("Invalid number of variables for the operation");
        }
        for (int i = 0; i < vars1->size(); i++) {
            if (vars1->at(i).code() != vars2->at(i).code()) {
                throw std::runtime_error("Invalid number of variables for the operation");
            }
        }
    }

    inline static void validateOneArgs(TokenList *vars) {
        if (vars->size() != 1) {
            throw std::runtime_error("Invalid number of variables for the operation");
        }
    }

    inline static void validateTwoArgs(TokenList *vars) {
        if (vars->size() != 2) {
            throw std::runtime_error("Invalid number of variables for the operation");
        }
    }

    inline static void validateThreeArgs(TokenList *vars) {
        if (vars->size() != 3) {
            throw std::runtime_error("Invalid number of variables for the operation");
        }
    }

    inline static void validateTwoOrMoreArgs(TokenList *vars) {
        if (vars->size() < 2) {
            throw std::runtime_error("Invalid number of variables for the operation");
        }
    }

    inline static void validateThreeOrMoreArgs(TokenList *vars) {
        if (vars->size() < 3) {
            throw std::runtime_error("Invalid number of variables for the operation");
        }
    }

    inline static std::vector<Symbol *> validateSymbols(SyntaxProg *prog, TokenList *vars) {
        std::vector<Symbol *> varSymbols;
        for (int i = 0; i < vars->size(); i++) {
            if (prog->hasVarSymbol(vars->at(i))) {
                varSymbols.push_back(dynamic_cast<VarSymbol *>(prog->lookup(vars->at(i))));
            } else {
                throw std::runtime_error("Variable is undefined");
            }
        }
        return varSymbols;
    }
};

#endif//DDOPERATION_HPP
