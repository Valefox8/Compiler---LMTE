#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "Token.h"

class Lexer{
    private:
        std::string source; // Input
        int position;       // current character

        Token ReadNumber();     // Pretty self expanitory 
        Token ReadIdentifierOrKeyword();    // Pretty self expanitory 

    public:
        Lexer(std::string source);      // Declares the lexer
        std::vector<Token> Tokenize();      // Tokenizes (returns a vector of tokens)
}



#endif