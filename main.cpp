#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Lexer.h"
#include "Token.h"
#include "Parser.h"
#include "Expr.h"
#include "CodeGen.h"

// TEST FILE
int main()
{
    // Source test
    std::string source = "1 mul (2 add 3) div 10";

    // Starts lexer
    Lexer lexer(source);

    // Tokenizes
    std::vector<Token> tokens = lexer.Tokenize();

    // Starts parser using the tokens generated previously
    Parser parser(tokens);

    // Generates parse tree
    std::unique_ptr<Expr> tree = parser.ParseExpression();

    // Creates an instance of code generator
    CodeGen generator;

    // Generates the output using the generate
    std::string output = generator.GenCode(tree.get());

    // Print the result and initial string
    std::cout << "Source:    " << source << std::endl;
    std::cout << "Generated: " << output << std::endl;

    return 0;
}