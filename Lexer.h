#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "Token.h"

class Lexer{
    private:
        std::string source;
        int position;

        Token ReadNumber();
        Token ReadIdentifierOrKeyword();

    public:
        Lexer(std::string source);
        std::vector<Token> Tokenize();
}



#endif