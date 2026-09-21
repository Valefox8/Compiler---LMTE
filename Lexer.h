/*
    Purpose:
        Read text file output, align codes to 
        keys or value for Parser to read

    Version Comtrol:
        No change for signature
        No added function

    Components:
        Variables:
            std::string source  -> Source code from text file, code that need to translate
            int position        -> Position of the character, it's character by character, not token by token
        
        Funcitons:    
            ReadNumber()        -> Read through value of integers create number token
            ReadIdOrKey()       -> Read through letters, if matches with any key, create corresponding key token, otherwise identifier token
            ReadWord()          -> Read letters in between '$', create word token
            ReadLetter()        -> Read letter in between '#', create letter token
            Comment()           -> Read any string in between '<!--' and '-->', create comment token
            checkforward()      -> Read whether the input string lenght is greater than total length, use for comment to check the scope
            Lexer()             -> Constructor
            Tokenize()          -> Read current character, match with keys
    
*/

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