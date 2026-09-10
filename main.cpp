#include <iostream>
#include <string>
#include <vector>
#include "Lexer.h"
#include "Token.h"
 

// LEXER TEST FILE
int main()
{
    // Source test
    std::string source = "1 add 678 mul 67 sub 6782";
 
    // Starts lexer
    Lexer lexer(source);

    // Tokenizes
    std::vector<Token> tokens = lexer.Tokenize();
 
    // Prints all tokens
    for (size_t i = 0; i < tokens.size(); i++)
    {
        std::cout << "Type: " << static_cast<int>(tokens[i].Type) << ", Value: " << tokens[i].Value << std::endl;
    }
 
    return 0;
}
 