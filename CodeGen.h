/*
    Purpose:
        Read expressions, translate expression token by token
        with python standard, output python codeline as string for
        write into python.py

    Version Comtrol:
        Old version:
            function: 
                std::string Operator(TokenType op);
                std::string GenCode(Expr* expr);
        
        New version:
            function add:
                std::string Iteration(int level, IterationExpr* expr);
                std::string ConditionalStatementStruct(int level, ConditionalStatementStructExpr* expr);
                std::string ConditionalStatement(int level, ConditionalStatementExpr* expr);
            
            function change:
                std::string GenCode(Expr* expr);  ->  std::string GenCode(int level, Expr* expr);


    Components:
        Variables:
            Int Level               -> represent the depth of the indention
            TokenTyp op             -> represent the operator in between two value or variable
            Expr* expr              -> any expression 
            IterationExpr* expr     -> expression that specifically has structure of: vector<unique_ptr<Expr>> -> Codelines
           
            ConditionalStatementStructExpr* expr ->
                expression that specifically has structure of: vector<unique_ptr<ConditionalStatementExpr>> -> ConditionalStatements
            
            ConditionalStatementExpr* expr ->
                expression that specifically has structure of: 
                    TokenType: Statement Type 
                    Expr: Condtion -> default nullptr
                    vector<unique_ptr<Expr>> : Codelines -> default {}
        
        Functions:
            Operator()
            Iteration()
            ConditionalStatementStruct()
            ConditionalStatement()
            GenCode()

        
*/

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