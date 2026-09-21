#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <string>
#include "Expr.h"

/*
    Purpose:
        Read Tokenise code, translate token language token by token
        with python standard, output python codeline as string for
        write into python.py

    Version Comtro:
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
        Int Level : represent the depth of the indention
        TokenTyp op : represent the operator in between two value or variable
        Expr* expr : any expression 
        IterationExpr* expr : expression that specifically has structure of: vector<unique_ptr<Expr>> -> Codelines
        ConditionalStatementStructExpr* expr : 
            expression that specifically has structure of: vector<unique_ptr<ConditionalStatementExpr>> -> ConditionalStatements
        ConditionalStatementExpr* expr : 
            expression that specifically has structure of: 
                TokenType: Statement Type 
                Expr: Condtion -> default nullptr
                vector<unique_ptr<Expr>> : Codelines -> default {}
        
*/

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