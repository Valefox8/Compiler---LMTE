#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <string>
#include "Expr.h"

class CodeGen
{
private:
    std::string Operator(TokenType op);
    std::string Indent(std::string text);

public:
    std::string GenCode(Expr* expr);
};

#endif