#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "TokenType.h"

struct Token
{
    TokenType Type;
    std::string Value;

    Token(TokenType type, std::string value)
    {
        Type = type;
        Value = value;
    }
};

#endif