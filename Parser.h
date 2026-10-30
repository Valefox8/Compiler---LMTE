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
#include <stdexcept>
#include <map>
#include <string>
#include <iostream>


#include "Token.h"
#include "Expr.h"
#include "SymbolEntry.h"


class Parser{
    private:
    std::vector<Token> tokens;      // The entire token list
    int position;                   // Tracking the index in tokens
    std::unique_ptr<ScopeExpr> GlobalScope = std::make_unique<ScopeExpr>();
    ScopeExpr* currentScope;
    bool inWhile;

    // Read token healper function 
    Token Peek(size_t pos); // Returns the next token without moving position
    Token Current();                // Returns the token at the position
    Token Advance();                // Returns the position token and increments the position

    // Arithmatic/Boolean helper functions
    std::unique_ptr<Expr> ParseTerm();          // Implements the term grammar rule for mul/div/mod tl group them before add/sub
    std::unique_ptr<Expr> ParseFactor();        //  Implements factor grammer rule for numbers, identifiers, etc. Called by parse term
    std::unique_ptr<Expr> ParseExpression();    // Implements the expressions and returns the expression tree
    std::unique_ptr<Expr> ParseStatement();     // Decides whether its an expression or a variable dec for parsing

    std::unique_ptr<Expr> ParseComparisonOperator();
    std::unique_ptr<Expr> ParseBooleanOperator();

    // Scope helper functions
    std::vector<std::unique_ptr<Expr>> ParseBlock();               // Reads a whole ': statements ;' scope
    FunctionDecExpr* CheckParentScope(ScopeExpr* parentscope);


    // Funciton helper funciton 
    std::vector<std::unique_ptr<ParameterExpr>> ParseParams();   // Reads a parameter list up to the ':'
    std::vector<std::unique_ptr<Expr>> ParseArgs(std::vector<ParameterExpr*> expected, std::string name); // Reads a '|' separated argument list and checks each type
    std::unique_ptr<FunctionDecExpr> ParseFunctionDec(SymbolKind symK, TokenType access = TokenType::Public);   // Parses a FUNCTION declaration
    std::unique_ptr<ReturnExpr> ParseReturn();        // Parses a 'leave' statement
    std::unique_ptr<FunctionCallExpr> ParseFunctionCall(ClassDecExpr* primaryClassDecExpr = nullptr);   // Parses an 'f' function call


    // Condictional/Iteration helper function
    std::unique_ptr<IterationExpr> ParseIteration();
    std::unique_ptr<HandbrakeExpr> ParseBreak();
    std::unique_ptr<ConditionalStatementStructExpr> ParseConditionalStatementStructure();
    std::unique_ptr<ConditionalStatementExpr> ParseConditionalStatement();


    // Variable declaration and its helper function
    std::unique_ptr<Expr> ParseGetValue(TokenType type);            // Get the value from a normal variable declaration
    std::unique_ptr<VarDecExpr> ParseVarDec();     // Self explainitory by the name
    std::unique_ptr<VarDecExpr> CreateAndRegisterVarDec(TokenType varType, std::string varName, std::unique_ptr<Expr> nameExpr, SymbolKind kind, ScopeExpr* targetScope, TokenType access = TokenType::Public); 
        // Adding the new var dec into symbol table and other functions


    // Type checker functions
    TokenType GetExprType(Expr* expr);  // Works out what type an expression gives back
    void CheckType(TokenType wanted, Expr* value, std::string what); // Throws an error if value isn't the wanted type

    
    // List Declartion functions
    std::unique_ptr<ListExpr> ParseList();         // also self explanitory by the name


    // Class functions
    std::unique_ptr<VarDecExpr> ParseAttributeDec();                    // Parses 'public Vnum selfish->x = ...'
    std::unique_ptr<AttReferenceExpr> ParseAttributeReference();        // selfish -> ATTRIBUTES or obj -> ATTRIBUTE 
    std::unique_ptr<ClassDecExpr> ParseClassDec(ClassDecExpr* parentClass =  nullptr); // Parses class declarations
    std::unique_ptr<ClassDecExpr> ParseInheritance();
    std::unique_ptr<AttReferenceExpr> ParseMethodOverride(std::string name);
    std::unique_ptr<ObjectDecExpr> ParseObjectDec(); // Parses 'CName obj = CName args'


    // Print funciton 
    std::unique_ptr<SayExpr> ParseSay(); // print implementation
    void CheckVarDecUsage (Expr* exprUsed, Expr* recordExpr);       // Helper func, mark if a variable is used

    public:
    Parser(std::vector<Token> tokens);      // Constructor 
    std::unique_ptr<ScopeExpr> ParseProgram();   // parses for expressions within a program

};

#endif