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
        Token ReadIdOrKey();    // Pretty self expanitory 
        Token ReadWord();     // also self expanitory
        Token ReadLetter();   // still self expanitory
        void Comment();         // Functionality for comments
        bool checkforward(std::string);     // Checks for string tag

    public:
        Lexer(std::string source);      // Declares the lexer
        std::vector<Token> Tokenize();      // Tokenizes (returns a vector of tokens)
};



#endif