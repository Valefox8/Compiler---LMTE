/*
    Purpose:
        Read text tokenize file, match with tokentype 
        and create corresponing expressions

    Version Comtrol:
        Added Function:
            std::unique_ptr<Expr> ParseIteration();
            std::unique_ptr<Expr> ParseConditionalStatementStructure();
            std::unique_ptr<Expr> ParseConditionalStatement();
            std::unique_ptr<Expr> ParseComparisonOperator();
            std::unique_ptr<Expr> ParseBooleanOperator();


    Components:
        Variables:
            std::vector<Token> tokens   -> Source Token from Lexer, code that need to translate into expressions
            int position                -> Position of the character, token by token
        
        Funcitons:    
            Token Current();                                                -> Read and return current position token 
            Token Advance();                                                -> Read and return current position token, position ++
            std::unique_ptr<Expr> ParseTerm();                              -> Build 'mul' 'div' etc level expr
            std::unique_ptr<Expr> ParseFactor();                            -> Build value and varaible level expr
            std::unique_ptr<Expr> ParseIteration();                         -> Build Iteration expr
            std::unique_ptr<Expr> ParseConditionalStatementStructure();     -> Build if elif else structure expr
            std::unique_ptr<Expr> ParseConditionalStatement();              -> Build if elif else statement expr
            std::unique_ptr<Expr> ParseComparisonOperator();                -> Build '<' '==' '>' etc level of expr
            std::unique_ptr<Expr> ParseBooleanOperator();                   -> Build 'and' and 'or' level of expr
            Parser(std::vector<Token> tokens);                              -> Constructor
            std::unique_ptr<Expr> ParseExpression();                        -> Build 'add' 'sub' level of expr
            std::vector<std::unique_ptr<Expr>> ParseProgram();              -> Go through all the token and generate expr
            std::unique_ptr<Expr> ParseStatement();                         -> Read TokenType and assign to corresponding level method
            std::unique_ptr<Expr> ParseVarDec();                            -> Build varaible declaration expr
            std::unique_ptr<Expr> ParseList();                              -> Build list declaration expr

    Code call hierechy:
        L1  -> <Conditional Strucutre> <Conditional Statement> <Iteration> 
        L2  -> <Boolean operator> 
        L3  -> <Comparison Operator>
        L4  -> <Expression>
        L5  -> <Term>
        L6  -> <Factor>
*/

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
    
    // DRAGAN CODE
    Token Peek(); // Returns the next token without moving position
    std::vector<std::pair<TokenType, std::string>> ParseParams();   // Reads a parameter list up to the ':'
    std::vector<std::unique_ptr<Expr>> ParseBlock();               // Reads a whole ': statements ;' scope
    std::unique_ptr<Expr> ParseMethod(TokenType access); // Parses one FUNCTION inside an access section

    // MARTIN CODE
    std::unique_ptr<Expr> ParseIteration();
    std::unique_ptr<Expr> ParseBreak();
    std::unique_ptr<Expr> ParseConditionalStatementStructure();
    std::unique_ptr<ConditionalStatementExpr> ParseConditionalStatement();
    std::unique_ptr<Expr> ParseComparisonOperator();
    std::unique_ptr<Expr> ParseBooleanOperator();


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