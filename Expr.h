#ifndef EXPR_H
#define EXPR_H


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

// The bigger fucntion for variable dec
class VarDecExpr : public Expr{
    public:
}

#endif