#include "CodeGen.h"
#include <stdexcept>

std::string GenCode(Expr* expr){

}


std::string Operator(TokenType op){
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