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

    std::ofstream outFile("python.py");   // open a file for writing

    if (!outFile.is_open())               // make sure file is there and opened
    {
        throw std::runtime_error("Couldnt open file");
    }

    for (size_t i = 0; i < trees.size(); i++)   // Same loop as before but to file
    {
        std::string output = generator.GenCode(0, trees[i].get());
        outFile << output << std::endl;   // write to the file instead of the terminal
    }

    outFile.close();   // Close file

    std::cout << "written to python.py" << std::endl;   // comfirmation message

    return 0;
}