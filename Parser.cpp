#include "Parser.h"
#include <stdexcept>


Parser::Parser(std::vector<Token> tokens){
    tokens = tokens;
    position = 0;
}

Token Parser::Current(){
    return tokens[position];
}

Token Parser::Advance(){
    Token token = tokens[position];
    position++;
    return token;
}

std::unique_ptr<Expr> Parser::ParseExpression(){}
std::unique_ptr<Expr> Parser::ParseTerm(){}
std::unique_ptr<Expr> Parser::ParseFactor(){}