#ifndef EXPR_H
#define EXPR_H

#include <vector>
#include <memory>
#include <string>
#include "TokenType.h"

// These are for the parser where it defines the tree node shapes

class Expr  // Base class
{
public:
    virtual ~Expr() {}
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

    IdentifierExpr(std::string name)
    {
        Name = name;
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

         VarDecExpr(TokenType varType, std::string name, std::unique_ptr<Expr> value)   // Var dec expression constructor
        {
            VarType = varType;
            Name = name;
            Value = std::move(value);
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


// Iteration components
class HandbrakeExpr : public Expr{
    public:
        std::string Value;
        HandbrakeExpr(){
            Value = "break";
        }
};

class IterationExpr : public Expr{
    public:
        std::vector<std::unique_ptr<Expr>> Codeline;

        IterationExpr(std::vector<std::unique_ptr<Expr>> codeline){
            Codeline = std::move(codeline);
        }
};

// Conditional statements
class ConditionalStatementStructExpr : public Expr{
    public:
        std::vector<std::unique_ptr<Expr>> ConditionalStatements;

        ConditionalStatementStructExpr(std::vector<std::unique_ptr<Expr>> conditionalStatements){
            ConditionalStatements = std::move(conditionalStatements);
        }
};

class ConditionalStatementExpr : public Expr{
    public:
        TokenType Type;
        std::unique_ptr<Expr> Condition;
        std::vector<std::unique_ptr<Expr>> Codeline;

        ConditionalStatementExpr(TokenType type, std::unique_ptr<Expr> cond = nullptr, std::vector<std::unique_ptr<Expr>> codeline){
            Type = type;
            Condition = std::move(cond);
            Codeline = std::move(codeline);
        }
};




#endif