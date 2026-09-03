#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

enum class TokenType
{
    Number,
    Identifier,
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    LeftParen,
    RightParen,
    EndOfFile
};

#endif