
/*
    Purpose:
        Classes for code structure, used for CodeGen to
        read and output as python standard code

    Version Comtrol:
        Added Classes:
            class HandbrakeExpr : public Expr
            class IterationExpr : public Expr
            class ConditionalStatementStructExpr : public Expr
            class ConditionalStatementExpr : public Expr

    Components:
        class Expr                                              -> Default destructor
        class NumberExpr : public Expr                          -> Value for number
        class IdentifierExpr : public Expr                      -> Value for identifier
        class BinaryExpr : public Expr                          -> Codeline structural as equation or comparison
        class WordExpr : public Expr                            -> Value for strings
        class LetterExpr : public Expr                          -> Value for characters
        class DigitExpr : public Expr                           -> Value for single digit
        class VarDecExpr : public Expr                          -> Variable declaration
        class ListExpr : public Expr                            -> List declaration and value assign
        class ListAtExpr : public Expr                          -> Equivelent to listname[position]
        class HandbrakeExpr : public Expr                       -> break
        class IterationExpr : public Expr                       -> while(True)
        class ConditionalStatementStructExpr : public Expr      -> if elif else structure
        class ConditionalStatementExpr : public Expr            -> if elif else statment
*/


#ifndef EXPR_H
#define EXPR_H

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "TokenType.h"
#include <utility>
#include"SymbolEntry.h"
using namespace std;

// These are for the parser where it defines the tree node shapes

class Expr  // Base class
{
public:
    virtual ~Expr() {}
};

class ClassDecExpr;
class MethodDecExpr;
class ParameterExpr;
class ObjectDecExpr;


// Scope defined to use the syntax table
class ScopeExpr : public Expr {
    public: 
        std::vector<std::unique_ptr<Expr>> Body; // The statements inside the scope
        std::map<std::string, std::unique_ptr<SymbolEntry>> Table;
        ScopeExpr* ParentScope;
        std::string Name;

        ScopeExpr (ScopeExpr* parentScope = nullptr)
        :   ParentScope(parentScope)
        {
        }

        bool Contain (const std::string& varName);
        bool ContainLocal (const std::string& varName);
        SymbolEntry* LookUp(const std::string& varName);
        SymbolEntry* LookUpLocal(const std::string& varName);
        void Insert(const std::string& varName, std::unique_ptr<SymbolEntry> entry);
        bool Update(const std::string& varName, std::unique_ptr<SymbolEntry> entry);
        ClassDecExpr* GetParentClass();
        std::string GetNames();
        void PrintSymbolTable() const;
        std::vector<ParameterExpr*> GetFunctionPara(std::string funcName = "SELF");

};

// these first ones are for arithmetic
class NumberExpr : public Expr  // Identifies a number
{
public:
    std::string Value;

    NumberExpr(std::string value)
    {
        Value = value;
    }
};

class IdentifierExpr : public Expr  // Identifies a variable/identifier
{
public:
    std::string Name;
    std::unique_ptr<Expr> Callee;

    IdentifierExpr(std::string name, std::unique_ptr<Expr> callee = nullptr)
    {
        Name = name;
        Callee = std::move(callee);
    }
};

class BinaryExpr : public Expr      // Identifies an expression // This is the structure for a 3 prong (number operator number) // Three address code
{
public:
    std::unique_ptr<Expr> Left;
    TokenType Operator;
    std::unique_ptr<Expr> Right;

    BinaryExpr(std::unique_ptr<Expr> left, TokenType op, std::unique_ptr<Expr> right)
    {
        Left = std::move(left);
        Operator = op;
        Right = std::move(right);
    }
};

// THESE NEXT ONES ARE FOR VARIABLES PRIMARILY
// same structure as above
class WordExpr : public Expr
{
public:
    std::string Value;

    WordExpr(std::string value)
    {
        Value = value;
    }
};

class LetterExpr : public Expr
{
public:
    std::string Value;

    LetterExpr(std::string value)
    {
        Value = value;
    }
};

// This one differs using a bool since real and cake is our true and false
class BooleanExpr : public Expr
{
public:
    bool Value;   // true for real, false for cake

    BooleanExpr(bool value)
    {
        Value = value;
    }
};

class CharacterExpr : public Expr
{
public:
    std::string Value;   // a single character, e.g. "q"

    CharacterExpr(std::string value)
    {
        Value = value;
    }
};

class DigitExpr : public Expr
{
public:
    std::string Value;   // a single digit, e.g. "5"

    DigitExpr(std::string value)
    {
        Value = value;
    }
};

// The bigger fucntion for variable dec
class VarDecExpr : public Expr{
    public:
        // The 3 parts of variable dec (type, name and value)
        TokenType VarType;              // Vnum, Vwords, Vboolean, Vletter, etc.
        std::string Name;               // the variable's identifier
        std::unique_ptr<Expr> Value;    // whatever is assigned - a NumberExpr, BinaryExpr, or a literal
        TokenType Access; // Public, Private or Protected
        bool IsAttribute;

        // Var dec expression constructor
         VarDecExpr(
            TokenType varType, 
            std::string name, 
            std::unique_ptr<Expr> value,
            TokenType access = TokenType::Public,
            bool isAttribute = false
        )   
        {
            VarType = varType;
            Name = name;
            Value = std::move(value);
            Access = access;
            IsAttribute = isAttribute;
        }

};

// THESE ARE FOR THE LIST IMPLEMENTATION
// List expression function
class ListExpr : public Expr{
    public:
        std::string Name;
        std::vector<std::unique_ptr<Expr>> Elements;

        ListExpr(std::string name, std::vector<std::unique_ptr<Expr>> elements){
            Name = name;
            Elements = std::move(elements);
        }
};

class ListAtExpr : public Expr{
    public:
        std::string ListName;
        std::unique_ptr<Expr> Index;

        ListAtExpr(std::string listName, std::unique_ptr<Expr> index)
        {
            ListName = listName;
            Index = std::move(index);
        }
};

class ListSizeExpr : public Expr{
    public:
    std::string ListName;

    ListSizeExpr(std::string listName)
    {
        ListName = listName;
    }
};

class IterationExpr : public ScopeExpr {
    public:
        IterationExpr(ScopeExpr* parentScope = nullptr) : ScopeExpr(parentScope){

        }
};

class HandbrakeExpr : public Expr{};

class ConditionalStatementExpr : public ScopeExpr{
    public:
        TokenType Type;
        std::unique_ptr<Expr> Condition;

        ConditionalStatementExpr(TokenType type, std::unique_ptr<Expr> cond = nullptr, ScopeExpr* parentScope = nullptr) : ScopeExpr(parentScope){
            Type = type;
            Condition = std::move(cond);
        }
};


class ConditionalStatementStructExpr : public Expr{
    public: 
        std::vector<std::unique_ptr<ConditionalStatementExpr>> ConditionalStatements;

        // use Expr because already restrict input expr to if else statements in ParseStatement()
        ConditionalStatementStructExpr(std::vector<std::unique_ptr<ConditionalStatementExpr>> conditionalStatements){
            ConditionalStatements = std::move(conditionalStatements);
        }
};

// A single parameter of a function or method
class ParameterExpr : public Expr {
    public: 
        std::string Name;
        TokenType Type;

        ParameterExpr (std::string name, TokenType type)
            : Name(name), Type(type)
            {}
};

// Is used for both normal function an method
class FunctionDecExpr : public ScopeExpr {
    public:
        TokenType Access;
        bool IsMethod;

        std::string Name; // Function name
        std::vector<std::unique_ptr<ParameterExpr>> Param;   // Parameters type and name
        TokenType ReturnType = TokenType::Nothing; // The type the function leaves, Nothing if it doesnt leave anything

        
        FunctionDecExpr
        ( 
            std::string name,
            ScopeExpr* parentScopeTable = nullptr,
            bool isMethod = false,
            TokenType access = TokenType::Public
        )   :   ScopeExpr(parentScopeTable),
                Name(name),
                Access(access),
                IsMethod(isMethod)
        {
        }

};

// 
class FunctionCallExpr : public Expr{
    public:
        FunctionDecExpr* FuncDecExpr;                   // The function  being called
        std::vector<std::unique_ptr<Expr>> Args;        // The arguments passed to it
        std::unique_ptr<Expr> Callee;                   // What object called the function, could be nothing or 'selfish' or an 'object' 

        FunctionCallExpr(FunctionDecExpr* funcDecExpr, std::vector<std::unique_ptr<Expr>> args, std::unique_ptr<Expr> callee = nullptr)
        {
            FuncDecExpr = funcDecExpr;
            Args = std::move(args);
            Callee = std::move(callee);
        }
};

class ReturnExpr : public Expr{     // Leave keyword
    public:
        std::unique_ptr<Expr> Value;

        ReturnExpr(std::unique_ptr<Expr> value)
        {
            Value = std::move(value);
        }
};

// For implementing classes
// Used to store class declaration expression, and is a scope with its own syntax table
class ClassDecExpr : public ScopeExpr {
    public:
        std::string Name; // Class name

        ClassDecExpr
        (
            std::string name, 
            ScopeExpr* parentScope = nullptr
        )   :   ScopeExpr(parentScope),
                Name(name)
        {
            std::vector<std::unique_ptr<Expr>> dummy;
            
            // Make it so that the selfish keyword refers to the class in the declaration
            std::unique_ptr<SymbolEntry> newEntry = std::make_unique<SymbolEntry>(
                "selfish",
                TokenType::Custom,
                SymbolKind::Variable,
                this
            );
            Insert("selfish", std::move(newEntry));
        }
};


// THESE ARE FOR READING AND WRITING VALUES

class AssignExpr : public Expr{ // a = a add 1
    public:
        std::unique_ptr<Expr> Target; // An IdentifierExpr or an AttributeExpr
        std::unique_ptr<Expr> Value; // Whatever it is being set to

        AssignExpr(std::unique_ptr<Expr> target, std::unique_ptr<Expr> value)
        {
            Target = std::move(target);
            Value = std::move(value);
        }
};

class ObjectDecExpr : public Expr{ // CMyVehicle Ford1 = CMyVehicle 100 | 10
    public:
        ClassDecExpr* ClassDefExpr; // The class being created
        std::string Name; // Object name
        std::vector<std::unique_ptr<Expr>> Args;

        ObjectDecExpr(ClassDecExpr* classDefExpr, std::string name, std::vector<std::unique_ptr<Expr>> args)
            :   ClassDefExpr(classDefExpr), Name(name), Args(std::move(args))
        {
        }
};

#endif