#include "CodeGen.h"
#include <stdexcept>

std::string CodeGen::Operator(TokenType op){
    if(op == TokenType::Add){
        return "+";
    }
    if(op == TokenType::Sub){
        return "-";
    }
    if(op == TokenType::Mul){
        return "*";
    }
    if(op == TokenType::Div){
        return "/";
    }
    if(op == TokenType::Mod){
        return "%";
    }

    throw std::runtime_error("Unknown op");
}

std::string CodeGen::GenCode(Expr* expr){
    if (NumberExpr* number = dynamic_cast<NumberExpr*>(expr))
    {
        return number->Value;
    }

    if (IdentifierExpr* identifier = dynamic_cast<IdentifierExpr*>(expr))
    {
        return identifier->Name;
    }

    if (BinaryExpr* binary = dynamic_cast<BinaryExpr*>(expr))
    {
        std::string left = GenCode(binary->Left.get());
        std::string right = GenCode(binary->Right.get());
        std::string op = Operator(binary->Operator);

        return "(" + left + " " + op + " " + right + ")";
    }

    throw std::runtime_error("Unknown expression type");
}


