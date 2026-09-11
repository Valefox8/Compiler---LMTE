#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Lexer.h"
#include "Token.h"
#include "Parser.h"
#include "Expr.h"

std::string OperatorSymbol(TokenType op)
{
    if (op == TokenType::Add) return "+";
    if (op == TokenType::Sub) return "-";
    if (op == TokenType::Mul) return "*";
    if (op == TokenType::Div) return "/";
    if (op == TokenType::Mod) return "%";

    return "?";
}

// Recursively prints the tree with indentation to show nesting
void PrintTree(Expr* expr, int depth)
{
    std::string indent(depth * 2, ' ');

    if (NumberExpr* number = dynamic_cast<NumberExpr*>(expr))
    {
        std::cout << indent << number->Value << std::endl;
        return;
    }

    if (IdentifierExpr* identifier = dynamic_cast<IdentifierExpr*>(expr))
    {
        std::cout << indent << identifier->Name << std::endl;
        return;
    }

    if (BinaryExpr* binary = dynamic_cast<BinaryExpr*>(expr))
    {
        std::cout << indent << OperatorSymbol(binary->Operator) << std::endl;
        PrintTree(binary->Left.get(), depth + 1);
        PrintTree(binary->Right.get(), depth + 1);
        return;
    }
}

// LEXER AND PARSER TEST FILE
int main()
{
    // Source test
    std::string source = "1 mul (2 add 3)";

    // Starts lexer
    Lexer lexer(source);

    // Tokenizes
    std::vector<Token> tokens = lexer.Tokenize();

    // Prints all tokens
    for (size_t i = 0; i < tokens.size(); i++)
    {
        std::cout << "Type: " << static_cast<int>(tokens[i].Type) << ", Value: " << tokens[i].Value << std::endl;
    }

    std::cout << "----- Parse Tree -----" << std::endl;

    // Starts parser using the tokens from above
    Parser parser(tokens);

    // Parses the full expression, builds the tree
    std::unique_ptr<Expr> tree = parser.ParseExpression();

    // Prints the tree structure
    PrintTree(tree.get(), 0);

    return 0;
}