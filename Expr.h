#ifndef EXPR_H
#define EXPR_H

#include <vector>
#include <memory>
#include <string>
#include "TokenType.h"
#include <utility>

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

class FunctionDecExpr : public Expr{
    public:
        std::string Name;                                        // Function name
        std::vector<std::pair<TokenType, std::string>> Params;   // Parameters type and name
        std::vector<std::unique_ptr<Expr>> Body;                 // The statements inside the scope
        bool IsMethod;                                           // Whether the function is a method
        
        FunctionDecExpr(std::string name, std::vector<std::pair<TokenType, std::string>> params, std::vector<std::unique_ptr<Expr>> body, bool isMethod = false)
        {
            Name = name;
            Params = std::move(params);
            Body = std::move(body);
            IsMethod = isMethod;
        }
};

class FunctionCallExpr : public Expr{
    public:
        std::string Name;                            // The function being called
        std::vector<std::unique_ptr<Expr>> Args;     // The arguments passed to it

        FunctionCallExpr(std::string name, std::vector<std::unique_ptr<Expr>> args)
        {
            Name = name;
            Args = std::move(args);
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
class AttributeDecExpr : public Expr{ // public Vnum self.speed = speed
    public:
        TokenType Access; // Public, Private or Protected
        TokenType VarType; // Vnum, Vwords, other variable types
        std::string Name; // The attribute name, without the self keyword
        std::unique_ptr<Expr> Value;

        AttributeDecExpr(TokenType access, TokenType varType, std::string name, std::unique_ptr<Expr> value)
        {
            Access = access;
            VarType = varType;
            Name = name;
            Value = std::move(value);
        }
};

class ClassDecExpr : public Expr{
    public:
        std::string Name; // Class name
        std::unique_ptr<Expr> Constructor; // A constructor or empty if the class doesn't have one

        ClassDecExpr(std::string name, std::unique_ptr<Expr> constructor)
        {
            Name = name;
            Constructor = std::move(constructor);
        }
};

// THESE ARE FOR READING AND WRITING VALUES
class AttributeExpr : public Expr{ // self.speed
    public:
        std::string ObjectName; // The thing before the dot, e.g. 'self'
        std::string Name; // The attribute after the dot, e.g. 'speed'

        AttributeExpr(std::string objectName, std::string name)
        {
            ObjectName = objectName;
            Name = name;
        }
};

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
#endif