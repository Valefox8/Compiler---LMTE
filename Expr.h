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


#endif