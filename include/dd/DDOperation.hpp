//
// Created by CanhDo on 2024/12/27.
//

#ifndef DDOPERATION_HPP
#define DDOPERATION_HPP

#include "core/SyntaxProg.hpp"
#include "ir/operations/StandardOperation.hpp"
#include "ast/UnitaryStmNode.hpp"

enum OperationType {
    SINGLE_TARGET_OP,
    SINGLE_TARGET_COP,
    SINGLE_TARGET_MCOP
};

struct GateInfo {
    Token token;
    OperationType type;
};

class DDOperation {
public:
    static UnitaryStmNode *makeOperation(SyntaxProg *prog, TokenList *vars1, GateInfo gateInfo, TokenList *vars2) {
        std::string opName(gateInfo.token.name());
        std::transform(opName.begin(), opName.end(), opName.begin(), [](unsigned char c) { return std::tolower(c); });
        qc::OpType type = qc::opTypeFromString(opName);
        validateArgs(vars1, vars2);
        UnitaryStmNode *unitaryStmNode;
        switch (gateInfo.type) {
            case SINGLE_TARGET_OP: {
                validateOneArgs(vars1);
                auto varSymbols = validateSymbols(prog, vars1);
                unitaryStmNode = new UnitaryStmNode(gateInfo.token.code(), type, {}, varSymbols);
                break;
            }
            case SINGLE_TARGET_COP: {
                validateTwoArgs(vars1);
                auto varSymbols = validateSymbols(prog, vars1);
                unitaryStmNode = new UnitaryStmNode(gateInfo.token.code(), type, {varSymbols.front()},
                                                    {varSymbols.back()});
                break;
            }
            case SINGLE_TARGET_MCOP: {
                validateTwoOrMoreArgs(vars1);
                auto varSymbols = validateSymbols(prog, vars1);
                std::vector<Symbol *> controls(varSymbols.begin(), varSymbols.end() - 1);
                unitaryStmNode = new UnitaryStmNode(gateInfo.token.code(), type, controls, {varSymbols.back()});
            }
        }
        return unitaryStmNode;
    }

    static void validateArgs(TokenList *vars1, TokenList *vars2) {
        if (vars1->size() != vars2->size()) {
            throw std::runtime_error("Invalid number of variables for single target operation");
        }
        for (int i = 0; i < vars1->size(); i++) {
            if (vars1->at(i).code() != vars2->at(i).code()) {
                throw std::runtime_error("Invalid number of variables for single target operation");
            }
        }
    }

    static void validateOneArgs(TokenList *vars) {
        if (vars->size() != 1) {
            throw std::runtime_error("Invalid number of variables for single target operation");
        }
    }

    static void validateTwoArgs(TokenList *vars) {
        if (vars->size() != 2) {
            throw std::runtime_error("Invalid number of variables for single target control operation");
        }
    }

    static void validateTwoOrMoreArgs(TokenList *vars) {
        if (vars->size() < 2) {
            throw std::runtime_error(
                "Invalid number of variables for single target multiple control operation");
        }
    }

    static std::vector<Symbol *> validateSymbols(SyntaxProg *prog, TokenList *vars) {
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

#endif //DDOPERATION_HPP
