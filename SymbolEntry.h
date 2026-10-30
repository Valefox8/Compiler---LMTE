#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include<map>
#include<string>
#include <iostream>
#include<stdexcept>
#include<memory>
#include<iostream>

#include"Expr.h"
#include"TokenType.h"

class Expr;


enum class SymbolKind
{
    Variable,
    Attribute,
    Object,
    Function,
    Method,
    Class,
    Parameter,
};


// Each entry in the symbol table
class SymbolEntry
{
    public:
        std::string Name;   
        TokenType Type;        
        SymbolKind SymK;        // Check if a symbol is a variable, a class declaration, a function or a parameter
        Expr* Expression;       // Points to the definition/expression of a statement
        TokenType Access;
        int DeclarationLine;    
                                
        SymbolEntry
        (
            std::string name, 
            TokenType type, 
            SymbolKind symK, 
            Expr* expression,
            TokenType access = TokenType::Public,
            int declarationLine = 0  
        ) 
            :   Name(name),
                Type(type),
                SymK(symK),
                Expression(expression),
                Access(access),
                DeclarationLine(declarationLine)
        {
        }
};



#endif