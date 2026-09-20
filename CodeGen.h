#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <string>
#include "Expr.h"

class CodeGen
{
private:
    std::string Operator(TokenType op);
    std::string Iteration(int level, IterationExpr* expr);
    std::string ConditionalStatementStruct(int level, ConditionalStatementStructExpr* expr);
    std::string ConditionalStatement(int level, ConditionalStatementExpr* expr);
    

public:
    std::string GenCode(int level, Expr* expr);
};

#endif