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
    inline static UnitaryStmNode *makeOperation(SyntaxProg *prog, GateInfo gateInfo, TokenList *vars, std::vector<qc::fp> *params, std::string& errorMsg) {
        std::string opName(gateInfo.token.name());
        std::transform(opName.begin(), opName.end(), opName.begin(), [](unsigned char c) { return std::tolower(c); });
        qc::OpType type = qc::opTypeFromString(opName);
        auto varSymbols = validateSymbols(prog, vars, errorMsg);
        if (!errorMsg.empty()) return nullptr;
        UnitaryStmNode *unitaryStmNode;
        auto parameters = params == nullptr ? std::vector<qc::fp>{} : *params;
        switch (gateInfo.type) {
            case SINGLE_TARGET_OP_WITH_ONE_PARAM:
                if (!validateOneParams(params, errorMsg)) return nullptr;
                goto SINGLE_TARGET_OP;
            case SINGLE_TARGET_OP_WITH_TWO_PARAM:
                if (!validateTwoParams(params, errorMsg)) return nullptr;
                goto SINGLE_TARGET_OP;
            case SINGLE_TARGET_OP_WITH_THREE_PARAM:
                if (!validateThreeParams(params, errorMsg)) return nullptr;
            case SINGLE_TARGET_OP: {
            SINGLE_TARGET_OP:
                if (!validateOneArgs(vars, errorMsg)) return nullptr;
                unitaryStmNode = new UnitaryStmNode(gateInfo.token.code(), type, {}, varSymbols, parameters);
                break;
            }
            case SINGLE_TARGET_COP_WITH_ONE_PARAM:
                if (!validateOneParams(params, errorMsg)) return nullptr;
                goto SINGLE_TARGET_COP;
            case SINGLE_TARGET_COP_WITH_TWO_PARAM:
                if (!validateTwoParams(params, errorMsg)) return nullptr;
                goto SINGLE_TARGET_COP;
            case SINGLE_TARGET_COP_WITH_THREE_PARAM:
                if (!validateThreeParams(params, errorMsg)) return nullptr;
            case SINGLE_TARGET_COP: {
            SINGLE_TARGET_COP:
                if (!validateTwoArgs(vars, errorMsg)) return nullptr;
                unitaryStmNode = new UnitaryStmNode(gateInfo.token.code(), type, {varSymbols.front()},
                                                    {varSymbols.back()}, parameters);
                break;
            }
            case SINGLE_TARGET_MCOP_WITH_ONE_PARAM:
                if (!validateOneParams(params, errorMsg)) return nullptr;
                goto SINGLE_TARGET_MCOP;
            case SINGLE_TARGET_MCOP_WITH_TWO_PARAM:
                if (!validateTwoParams(params, errorMsg)) return nullptr;
                goto SINGLE_TARGET_MCOP;
            case SINGLE_TARGET_MCOP_WITH_THREE_PARAM:
                if (!validateThreeParams(params, errorMsg)) return nullptr;
                goto SINGLE_TARGET_MCOP;
            case SINGLE_TARGET_MCOP: {
            SINGLE_TARGET_MCOP:
                if (!validateTwoOrMoreArgs(vars, errorMsg)) return nullptr;
                std::vector<Symbol *> controls(varSymbols.begin(), varSymbols.end() - 1);
                unitaryStmNode = new UnitaryStmNode(gateInfo.token.code(), type, controls, {varSymbols.back()},
                                                    parameters);
                break;
            }
            case TWO_TARGET_OP_WITH_ONE_PARAM:
                if (!validateOneParams(params, errorMsg)) return nullptr;
                goto TWO_TARGET_OP;
            case TWO_TARGET_OP_WITH_TWO_PARAM:
                if (!validateTwoParams(params, errorMsg)) return nullptr;
                goto TWO_TARGET_OP;
            case TWO_TARGET_OP: {
            TWO_TARGET_OP:
                if (!validateTwoArgs(vars, errorMsg)) return nullptr;
                unitaryStmNode = new UnitaryStmNode(gateInfo.token.code(), type, {},
                                                    varSymbols, parameters);
                break;
            }
            case TWO_TARGET_COP_WITH_ONE_PARAM:
                if (!validateOneParams(params, errorMsg)) return nullptr;
                goto TWO_TARGET_COP;
            case TWO_TARGET_COP_WITH_TWO_PARAM:
                if (!validateTwoParams(params, errorMsg)) return nullptr;
                goto TWO_TARGET_COP;
            case TWO_TARGET_COP: {
            TWO_TARGET_COP:
                if (!validateThreeArgs(vars, errorMsg)) return nullptr;
                std::vector<Symbol *> targets(varSymbols.end() - 2, varSymbols.end());
                unitaryStmNode = new UnitaryStmNode(gateInfo.token.code(), type, {varSymbols.front()}, targets,
                                                    parameters);
                break;
            }
            case TWO_TARGET_MCOP_WITH_ONE_PARAM:
                if (!validateOneParams(params, errorMsg)) return nullptr;
                goto TWO_TARGET_MCOP;
            case TWO_TARGET_MCOP_WITH_TWO_PARAM:
                if (!validateTwoParams(params, errorMsg)) return nullptr;
                goto TWO_TARGET_MCOP;
            case TWO_TARGET_MCOP: {
            TWO_TARGET_MCOP:
                if (!validateThreeOrMoreArgs(vars, errorMsg)) return nullptr;
                std::vector<Symbol *> controls(varSymbols.begin(), varSymbols.end() - 2);
                std::vector<Symbol *> targets(varSymbols.end() - 2, varSymbols.end());
                unitaryStmNode = new UnitaryStmNode(gateInfo.token.code(), type, controls, targets, parameters);
                break;
            }
        }
        return unitaryStmNode;
    }

    inline static bool validateOneParams(std::vector<qc::fp> *params, std::string &errorMsg) {
        if (params == nullptr || params->size() != 1) {
            std::cout << "\e[31mInvalid number of parameters for the operation\e[0m" << std::endl;
            return false;
        }
        return true;
    }

    inline static bool validateTwoParams(std::vector<qc::fp> *params, std::string &errorMsg) {
        if (params == nullptr || params->size() != 2) {
            std::cout << "\e[31mInvalid number of parameters for the operation\e[0m" << std::endl;
            return false;
        }
        return true;
    }

    inline static bool validateThreeParams(std::vector<qc::fp> *params, std::string &errorMsg) {
        if (params == nullptr || params->size() != 3) {
            std::cout << "\e[31mInvalid number of parameters for the operation\e[0m" << std::endl;
            return false;
        }
        return true;
    }

    inline static bool validateArgs(TokenList *vars1, TokenList *vars2, std::string &errorMsg) {
        if (vars1->size() != vars2->size()) {
            errorMsg = "Invalid number of variables for the operation";
            return false;
        }
        for (int i = 0; i < vars1->size(); i++) {
            if (vars1->at(i).code() != vars2->at(i).code()) {
                errorMsg = "Variables are not the same for the operation";
                return false;
            }
        }
        return true;
    }

    inline static bool validateOneArgs(TokenList *vars, std::string &errorMsg) {
        if (vars->size() != 1) {
            errorMsg = "The number of variables should be one for the operation";
            return false;
        }
        return true;
    }

    inline static bool validateTwoArgs(TokenList *vars, std::string &errorMsg) {
        if (vars->size() != 2) {
            errorMsg = "The number of variables should be two for the operation";
            return false;
        }
        return true;
    }

    inline static bool validateThreeArgs(TokenList *vars, std::string &errorMsg) {
        if (vars->size() != 3) {
            errorMsg = "The number of variables should be three for the operation";
            return false;
        }
        return true;
    }

    inline static bool validateTwoOrMoreArgs(TokenList *vars, std::string &errorMsg) {
        if (vars->size() < 2) {
            errorMsg = "The number of variables should be less than two for the operation";
            return false;
        }
        return true;
    }

    inline static bool validateThreeOrMoreArgs(TokenList *vars, std::string &errorMsg) {
        if (vars->size() < 3) {
            errorMsg = "The number of variables should be less than three for the operation";
            return false;
        }
        return true;
    }

    inline static std::vector<Symbol *> validateSymbols(SyntaxProg *prog, TokenList *vars, std::string &errorMsg) {
        std::vector<Symbol *> varSymbols;
        for (int i = 0; i < vars->size(); i++) {
            if (prog->hasVarSymbol(vars->at(i))) {
                varSymbols.push_back(dynamic_cast<VarSymbol *>(prog->lookup(vars->at(i))));
            } else {
                errorMsg = "Variable " + std::string(vars->at(i).name()) + " is undefined";
            }
        }
        return varSymbols;
    }
};

#endif//DDOPERATION_HPP
