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
    Equals,

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

    // List helpers
    Quote,      // " for the start and end of the list
    Separator,  // '|' for separating list items
    Dot,         // "." for .size() and .at()
    LeftBracket,   // [ for .at
    RightBracket,  // ] for .at

    // Function scope 
    Comma, // "," separates statements in  a function
    Colon, // ":" begins the scope for a function
    Semicolon, // ";" ends a statement in a function


    // Defining Functions
    Function, // "function" keyword
    Leave,    // "leave" keyword
    Call,    // "f" keyword to begin a function call
};

#endif