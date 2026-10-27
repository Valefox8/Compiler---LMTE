#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include<map>
#include<string>
#include <iostream>
#include<stdexcept>
#include<memory>
#include<iostream>
#include"Expr.h"

class Expr;


enum class SymbolKind
{
    Variable,
    Attribute,
    Object,
    Function,
    Method,
    Class,
    Parameter,      // I feel like its very similar to variables, maybe we could delete it
};

// TODO: change the code to use this instead of TokenType plz, we should keep them seperated
enum class DataType
{
    Vnum,
    Vwords,
    Vboolean,
    Vletter,
    VCharacter,
    VDigit,
    List,
    Custom,
};


class SymbolEntry
{
    public:
        std::string Name;   
        TokenType Type;         //TODO: Add type checking for the return type of a function 
        SymbolKind SymK;        // Check if a symbol is a variable, a class declaration, a function or a parameter
        Expr* Expression;       // Points to the definition/expression of a statement
        TokenType Access;
        int DeclarationLine;    //TODO: this would have been very useful had we implemented it lol. It would make bug fixes so much easier omg 
                                // We could not implement DeclarationLine i guess. Only useful for BUG FIXING (omg). it seems simple tho
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