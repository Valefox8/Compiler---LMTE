#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <memory>
#include "Token.h"
#include "Expr.h"

class Parser{
    private:
    std::vector<Token> tokens;      // The entire token list
    int position;                   // Tracking the index in tokens

    Token Current();                // Returns the token at the position
    Token Advance();                // Returns the position token and increments the position
    std::unique_ptr<Expr> ParseTerm();          // Implements the term grammar rule for mul/div/mod tl group them before add/sub
    std::unique_ptr<Expr> ParseFactor();        //  Implements factor grammer rule for numbers, identifiers, etc. Called by parse term

    public:
    Parser(std::vector<Token> tokens);      // Constructor 
    std::unique_ptr<Expr> ParseExpression();    // Implements the expressions and returns the expression tree
};


#endif