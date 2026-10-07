/*
    Purpose:
        Enum Class for reference TokenType

    Version Comtrol:
        No added function
        No changed function

    Components:
        All type of Token
*/

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

    // Conditional statements
    IguessIf,   // if(<condition>){}
    Guessthis,  // else if(<condition){}
    Guessnot,   // else{}
    
    // Comparison Expression
    Equalto, // ==
    Lessthan, // <
    Morethan, // >
    Istotallydefinitelynotequalto, // !=
    Equallessthan, // <=
    Equalmorethan, // >=

    // Boolean Operator
    Dna, // and
    Ro, // or

    // Iterations
    Forwhencake, // while(True){}
    Handbrake,   // break

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

    // Classes
    Self,  // "SELF" keyword
    Public,  // "public" access modifier
    Private,  // "private" access modifier
    Protected,  // "protected" access modifier
    Arrow, // "->" for calling methods and reading attributes on an object
};

#endif