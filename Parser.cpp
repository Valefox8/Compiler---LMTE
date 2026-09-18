#include "Parser.h"
#include <stdexcept>
#include "TokenType.h"


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


//create a new function for boolean operators
std::unique_ptr<Expr> Parser::ParseBooleanOperator(){
    std::unique_ptr<Expr> left = ParseComparisonOperator();

    while(Current().Type == TokenType::Dna || Current().Type == TokenType::Ro)
    {
        TokenType op = Advance().Type;
        std::unique_ptr<Expr> right = ParseComparisonOperator();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }
}

// create a new function for conditional statement
std::unique_ptr<Expr> Parser::ParseComparisonOperator(){
    std::unique_ptr<Expr> left = ParseExpression();

    while(Current().Type == TokenType::Equalto || Current().Type == TokenType::Equallessthan || Current().Type == TokenType::Istotallydefinitelynotequalto ||
        Current().Type == TokenType::Equalmorethan || Current().Type == TokenType::Lessthan || Current().Type == TokenType::Morethan)
    {
        TokenType op = Advance().Type;
        std::unique_ptr<Expr> right = ParseExpression();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }
    
    return left;
}
 

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

    if (token.Type == TokenType::Quote)                          // Check if its the start of a list
    {
        return ParseList();
    }

    if (token.Type == TokenType::Number)    // Check if number
    {
        Advance();  // Advance position
        return std::make_unique<NumberExpr>(token.Value);   // Return parsed value
    }

    if (token.Type == TokenType::Identifier)    //Check if the token is an identifier
    {
        Advance();  // Advance position
        std::string name = token.Value;     // gets list name

        if (Current().Type == TokenType::Dot){
            Advance();      // Move on from the '.'
            Token methodToken = Advance();      // Get method

            if (methodToken.Value == "size")
            {
                Advance();      // '('
                Advance();      // ')'
                return std::make_unique<ListSizeExpr>(name);    
            }

            if (methodToken.Value == "at")
            {
                Advance();  // '['
                std::unique_ptr<Expr> index = ParseExpression();    // gets the value in between the brackets (the at value)
                Advance();  // ']'
                return std::make_unique<ListAtExpr>(name, std::move(index));
            }

            throw std::runtime_error("Unknown method: " + methodToken.Value);
        }


        return std::make_unique<NumberExpr>(name);   // Return parsed value
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

std::vector<std::unique_ptr<Expr>> Parser::ParseProgram(){
    std::vector<std::unique_ptr<Expr>> statements;

    while (Current().Type != TokenType::EndOfFile)
    {
        statements.push_back(ParseStatement());   // changed from ParseExpression() so we can use statements too
    }

    return statements;
}

std::unique_ptr<Expr> Parser::ParseStatement(){

    TokenType current = Current().Type;     // Gets the current token type 

    if (current == TokenType::Vnum || current == TokenType::Vwords || current == TokenType::Vboolean || current == TokenType::Vletter || current == TokenType::VCharacter || current == TokenType::VDigit)  // If the token type is a type identifier, call variable declaration
    {
        return ParseVarDec();
    }

    // iteration check
    else if (current == TokenType::Forwhencake)
    {
        /* code */
    }

    // conditional statement check
    else if (current == TokenType::IguessIf || current == TokenType::Guessthis || current == TokenType::Guessnot )
    {
        /* code */
    }
    

    return ParseBooleanOperator();       // Otherwise call expression (arithemtic) for now
}      

// Follows grammer rule     <VariableDeclaration> ->	<VariableType> Identifier = <Expression>

std::unique_ptr<Expr> Parser::ParseVarDec(){
    TokenType varType = Advance().Type;     // Saves the type and moves to next value
    Token nameToken = Advance();          // saves the name as a token
    std::string name = nameToken.Value;     // Creates a name string based off the value just found from the token

    Advance();                            // Moves on past the =

    std::unique_ptr<Expr> value;    // initialises the value

    if(varType == TokenType::Vwords){
        Token lit = Advance();  // Gets the literal and moves position
        value = std::make_unique<WordExpr>(lit.Value);  // Saves this value 
    }

    else if(varType == TokenType::Vletter)
    {
        Token lit = Advance();          // Gets the literal and moves position
        value = std::make_unique<LetterExpr>(lit.Value);    // Saves this value 
    }

    else if(varType == TokenType::Vboolean)
    {
        Token lit = Advance();          // Gets the literal and moves position
        bool boolValue = NULL;
        if(lit.Type == TokenType::Real){
            boolValue = true;
        }
        else if(lit.Type == TokenType::Cake){
            boolValue = false;
        }

        value = std::make_unique<BooleanExpr>(boolValue);    // Saves this value 
    }

    else if(varType == TokenType::VCharacter){
        Token lit = Advance();          // Gets the literal and moves position

        if(lit.Value.length() != 1){
            throw std::runtime_error("Should be length 1");  // As per the production rules
        }
        value = std::make_unique<CharacterExpr>(lit.Value);
    }

    else if(varType == TokenType::VDigit){
        Token lit = Advance();          // Gets the literal and moves position
        
        if(lit.Type != TokenType::Number || lit.Value.length() != 1)        // Digit must be a number of length 1 as per the production rules
        {
            throw std::runtime_error("Should be length 1 and a number gangalang");
        }
        value = std::make_unique<DigitExpr>(lit.Value);

    }

    else if(varType == TokenType::Vnum){
        value = ParseExpression();      // Vnum can be a whole expression or just a value which is covered in parse expression
    }

    else{
        throw std::runtime_error("Thats not a type lil bro lock in");   // Throws an error if its not a valid type
    }

    return std::make_unique<VarDecExpr>(varType, name, std::move(value));       // returns the expression created
 }

 // List -> '"' Identifier '|' <ElementList> '"'
 //<ElementList>	-> 	<Element> || <Element> ‘|’ <ElementList>

std::unique_ptr<Expr> Parser::ParseList(){
    Advance();      // Move past the first " 

    Token nameToken = Advance();    // Get the list name and move on
    std::string name = nameToken.Value;     // Save name as string

    Advance();  // Move past the '|'

    std::vector<std::unique_ptr<Expr>> elements;    // Create an elements vector
    elements.push_back(ParseExpression());      // Push first element (should always be at least 1 element)
    

    while (Current().Type == TokenType::Separator)  // Loops for amount of elements in the list
    {
        Advance();   // move past '|'
        elements.push_back(ParseExpression());   // next element
    }

    Advance();   // move past "

    return std::make_unique<ListExpr>(name, std::move(elements));   // Make a list expression using the name and element vector

}