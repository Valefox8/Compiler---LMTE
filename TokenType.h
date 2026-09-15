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
    EndOfFile,

    // Variable types
    Vnum,
    Vwords,
    Vboolean,
    Vletter,
    VCharacter,
    VDigit,

    // Literal types
    Real,
    Cake,

    // Literal tokens
    Word,     // $...$ // as per grammar
    Letter,   // #...# // as per grammar
};

#endif