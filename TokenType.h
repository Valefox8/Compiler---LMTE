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
    Equals, // assigne value

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
};

#endif