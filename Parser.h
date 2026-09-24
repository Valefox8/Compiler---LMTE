#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <memory>
#include "Token.h"
#include "Expr.h"
#include <map>
#include <string>

class Parser{
    private:
    std::vector<Token> tokens;      // The entire token list
    int position;                   // Tracking the index in tokens
    std::map<std::string, int> functionArity;   // Remembers how many parameters each function was declared with

    Token Current();                // Returns the token at the position
    Token Advance();                // Returns the position token and increments the position
    std::unique_ptr<Expr> ParseTerm();          // Implements the term grammar rule for mul/div/mod tl group them before add/sub
    std::unique_ptr<Expr> ParseFactor();        //  Implements factor grammer rule for numbers, identifiers, etc. Called by parse term
    Token Peek(); // Returns the next token without moving position
    std::vector<std::pair<TokenType, std::string>> ParseParams();   // Reads a parameter list up to the ':'
    std::vector<std::unique_ptr<Expr>> ParseBlock();               // Reads a whole ': statements ;' scope

    public:
    Parser(std::vector<Token> tokens);      // Constructor 
    std::unique_ptr<Expr> ParseExpression();    // Implements the expressions and returns the expression tree
    std::vector<std::unique_ptr<Expr>> ParseProgram();   // parses for expressions within a program
    std::unique_ptr<Expr> ParseStatement();              // Decides whether its an expression or a variable dec for parsing
    std::unique_ptr<Expr> ParseVarDec();     // Self explainitory by the name
    std::unique_ptr<Expr> ParseList();         // also self explanitory by the name 
    std::unique_ptr<Expr> ParseFunctionDec();   // Parses a FUNCTION declaration
    std::unique_ptr<Expr> ParseReturn();        // Parses a 'leave' statement
    std::unique_ptr<Expr> ParseFunctionCall();   // Parses an 'f' function call
    std::unique_ptr<Expr> ParseClassDec(); // Parses class declarations
    std::unique_ptr<Expr> ParseAttributeDec(); // Parses 'public Vnum self.x = ...'
};


#endif