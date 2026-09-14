#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include "Lexer.h"
#include "Token.h"
#include "Parser.h"
#include "Expr.h"
#include "CodeGen.h"


std::string ReadFile(std::string path){
        std::ifstream file(path);

    if (!file.is_open())
    {
        throw std::runtime_error("Couldnt open file lil bro");
    }

    std::string source;
    std::string line;

    while (std::getline(file, line))
    {
        source += line + "\n";
    }

    return source;
}


// TEST FILE
int main()
{
    std::string path = "test.txt";   // hardcoded file path - change this to whatever you want
    std::string source = ReadFile(path);

    // Starts lexer
    Lexer lexer(source);

    // Tokenizes
    std::vector<Token> tokens = lexer.Tokenize();

    // Starts parser using the tokens generated previously
    Parser parser(tokens);

    // generates the trees for each line of the program
    std::vector<std::unique_ptr<Expr>> trees = parser.ParseProgram();

    // Creates an instance of code generator
    CodeGen generator;

    // Print the result and initial string
    std::cout << "Source:    " << source << std::endl;
    std::cout << "Generated: " << std::endl;

    for (size_t i = 0; i < trees.size(); i++)   // loop over each tree
    {
        std::string output = generator.GenCode(trees[i].get()); // generate code
        std::cout << output << std::endl;   // print code
    }

    return 0;
}