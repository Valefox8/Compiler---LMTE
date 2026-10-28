/*
    Purpose:
        class for token, 
        Lexer use source code to generate token
        Parser use token to generate expr

    Version Comtrol:
        No added function
        No changed function

    Components:
        Variables:
            TokenType Type      -> Type of Token, reference from enum class TokenType
            std::string Value   -> Value of the token

        Funcitons:
            Token()             -> Constructor
*/

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "TokenType.h"

struct Token
{
    TokenType Type;
    std::string Value;
    int LineUsed;

    Token(TokenType type, std::string value, int lineUsed = 0)
    {
        Type = type;
        Value = value;
        LineUsed = lineUsed;
    }
};

#endif