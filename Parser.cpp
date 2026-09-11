#include "Parser.h"
#include <stdexcept>


Parser::Parser(std::vector<Token> tokens){
    this->tokens = tokens;    // Initialise tokens
    this->position = 0;       // Initialise position
}

Token Parser::Current(){
    return tokens[position];        // Return current token in position
}

Token Parser::Advance(){
    Token token = tokens[position];     // Save token inposition
    position++;                         // Advance position
    return token;                       // Return token
}


// These follow production rules

//AddOperator -> add | sub
//MulOperator -> mul | div | mod 
//Expression -> Term | Expression AddOperator Term 
//Term -> Factor | Term MulOperator Factor
//Factor -> Digit* | Identifier | '(' Expression ')' 

// The parse tree is build off the rules where theres an expresion, a term and a factor, where they each get lower.

std::unique_ptr<Expr> Parser::ParseExpression(){
    std::unique_ptr<Expr> left = ParseTerm();   // Parse the first Term 

    while (Current().Type == TokenType::Add || Current().Type == TokenType::Sub)   // Loop while next token is add/sub
    {
        TokenType op = Advance().Type;      // get the operator, remember which one
        std::unique_ptr<Expr> right = ParseTerm();   // Parse the next Term as the right side
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));   // Combine into new node, replace left
    }

    return left;    // Return final result // this will be a term or tree from above loop
}


std::unique_ptr<Expr> Parser::ParseTerm(){
    std::unique_ptr<Expr> left = ParseFactor();    // Parse the first Factor 

    while (Current().Type == TokenType::Mul || Current().Type == TokenType::Div || Current().Type == TokenType::Mod)   // Loop while next token is mul/div/mod
    {
        TokenType op = Advance().Type;      // get the operator, remember which one
        std::unique_ptr<Expr> right = ParseFactor();   // Parse the next Factor as the right side
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));   // Combine into new node, replace left
    }

    return left;   // Return final result - single Factor or nested tree
}


std::unique_ptr<Expr> Parser::ParseFactor(){
    Token token = Current();    // Get current token
    if (token.Type == TokenType::Number)    // Check if number
    {
        Advance();  // Advance position
        return std::make_unique<NumberExpr>(token.Value);   // Return parsed value
    }

    if (token.Type == TokenType::Identifier)    //Check if the token is an identifier
    {
        Advance();  // Advance position
        return std::make_unique<IdentifierExpr>(token.Value); // Return parsed value
    }

    if (token.Type == TokenType::LeftParen)
    {
        Advance();      // Advance to next position
        std::unique_ptr<Expr> inner = ParseExpression();    // Parse the expression with the brackets
        Advance();  // Adavnce out of the brackets
        return inner;   // Return that expression of the brackets
    }

    throw std::runtime_error("Syntax error: unexpected token");
}