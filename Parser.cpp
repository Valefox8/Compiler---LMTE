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

Token Parser::Peek(){
    if (position + 1 >= (int)tokens.size()) // Stops reading before the end of the endoffile token
    {
        return tokens.back();
    }

    return tokens[position + 1];
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

    if (token.Type == TokenType::Quote)                          // Check if its the start of a list
    {
        return ParseList();
    }
    
    if (token.Type == TokenType::Call)                           // Check if its the start of a function call
    {
        return ParseFunctionCall();
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

            return std::make_unique<AttributeExpr>(name, methodToken.Value); // Anything else after a dot is an attribute like self.speed
        }


        return std::make_unique<IdentifierExpr>(name); // A variable name
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

        if (current == TokenType::Function)   // Function declaration
        {
            return ParseFunctionDec();
        }

        if (current == TokenType::Leave)      // Return statement
        {
            return ParseReturn();
        }

        if (current == TokenType::Identifier && Peek().Type == TokenType::Colon) // CName is a class
        {
            return ParseClassDec();
        }

        if (current == TokenType::Public || current == TokenType::Private || current == TokenType::Protected)   // Attribute declaration
        {
            return ParseAttributeDec();
        }

        std::unique_ptr<Expr> expr = ParseExpression(); // Could be an expression, or the target of an assignment

        if (Current().Type == TokenType::Equals) // It was a target, so this is a reassignment
        {
            if (dynamic_cast<IdentifierExpr*>(expr.get()) == nullptr && dynamic_cast<AttributeExpr*>(expr.get()) == nullptr)
            {
                throw std::runtime_error("You cannot assign to that");
            }

            Advance(); // Move past the '='

            std::unique_ptr<Expr> value = ParseExpression(); // The new value
            return std::make_unique<AssignExpr>(std::move(expr), std::move(value));
        }

        return expr;
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

// Follows grammer rule   <ParameterList> -> <Parameter> | <Parameter> '|' <ParameterList> | NOTHING
std::vector<std::pair<TokenType, std::string>> Parser::ParseParams(){
    std::vector<std::pair<TokenType, std::string>> params;

    while (Current().Type != TokenType::Colon)   // Read parameters until the scope opens
    {
        if (Current().Type == TokenType::EndOfFile)
        {
            throw std::runtime_error("Non closed scope: missing ':'");
        }

        TokenType paramType = Advance().Type;       // parameter type
        std::string paramName = Advance().Value;    // parameter name
        params.push_back({paramType, paramName});

        if (Current().Type == TokenType::Separator)   // '|' between parameters
        {
            Advance();
        }
    }

    return params;
}

// Reads a whole scope from ':' <StatementList> ';'

std::vector<std::unique_ptr<Expr>> Parser::ParseBlock(){
    Advance();      // Move past the ':'

    std::vector<std::unique_ptr<Expr>> body;

    while (Current().Type != TokenType::Semicolon)   // Read statements until the scope closes
    {
        if (Current().Type == TokenType::EndOfFile)
        {
            throw std::runtime_error("Non closed scope: missing ';'");
        }

        body.push_back(ParseStatement());

        if (Current().Type == TokenType::Comma)   // ',' between statements
        {
            Advance();
        }
    }

    Advance();      // Move past the ';'

    return body;
}

// Follows grammer rule   <FunctionDeclaration> -> FUNCTION Identifier <ParameterList>: <StatementList>;

std::unique_ptr<Expr> Parser::ParseFunctionDec(){
    Advance(); // Move past FUNCTION

    Token nameToken = Advance(); // Get the function name
    std::string name = nameToken.Value;

    std::vector<std::pair<TokenType, std::string>> params = ParseParams();

    functionArity[name] = (int)params.size(); // Save the parameter count before the body, so a function can call itself

    std::vector<std::unique_ptr<Expr>> body = ParseBlock();

    return std::make_unique<FunctionDecExpr>(name, std::move(params), std::move(body));
}

// Follows grammer rule   <ReturnStatement> -> leave <Expression>
std::unique_ptr<Expr> Parser::ParseReturn(){
    Advance();      // Move past leave
    return std::make_unique<ReturnExpr>(ParseExpression());
}

// Follows grammer rule   <FunctionCall> -> f <FunctionName> <ArgumentList>

std::unique_ptr<Expr> Parser::ParseFunctionCall(){
    Advance();      // Move past the 'f'

    Token nameToken = Advance();        // Get the function name
    std::string name = nameToken.Value;

    if (functionArity.count(name) == 0)     // The function was never declared
    {
        throw std::runtime_error("Cannot Recognize " + name);
    }

    int expected = functionArity[name];     // How many arguments this function takes

    std::vector<std::unique_ptr<Expr>> args;

    // Read exactly as many arguments as the declaration had parameters.
    for (int i = 0; i < expected; i++)
    {
        if (i > 0)
        {
            if (Current().Type != TokenType::Separator)
            {
                throw std::runtime_error("Function " + name + " did not get enough arguments");
            }

            Advance();   // Move past the '|'
        }

        args.push_back(ParseExpression());
    }

    return std::make_unique<FunctionCallExpr>(name, std::move(args));
}

// Follows grammer rule   <Methods> -> <FunctionDeclaration> | <FunctionDeclaration> <Methods>
std::unique_ptr<Expr> Parser::ParseMethod(TokenType access){
    if (Current().Type != TokenType::Function)
    {
        throw std::runtime_error("Only FUNCTION declarations are allowed inside an access section");
    }

    Advance(); // Move past FUNCTION

    Token nameToken = Advance(); // Get the method name
    std::string name = nameToken.Value;

    std::vector<std::pair<TokenType, std::string>> params = ParseParams();
    std::vector<std::unique_ptr<Expr>> body = ParseBlock();

    return std::make_unique<FunctionDecExpr>(name, std::move(params), std::move(body), true, access);
}

// Follows grammer rule   <ClassDeclaration> -> <ClassName> : <ClassMembers> ;
std::unique_ptr<Expr> Parser::ParseClassDec(){
    Token nameToken = Advance();        // Get the class name
    std::string name = nameToken.Value;

    if (name.length() < 2 || name[0] != 'C')
    {
        throw std::runtime_error("Class names must start with C: " + name);
    }

    Advance(); // Move past the ':'

    std::unique_ptr<Expr> constructor;

    if (Current().Type == TokenType::Function && Peek().Type == TokenType::Self)
    {
        Advance(); // Move past FUNCTION
        Advance(); // Move past SELF

        std::vector<std::pair<TokenType, std::string>> params = ParseParams();
        std::vector<std::unique_ptr<Expr>> body = ParseBlock();

        constructor = std::make_unique<FunctionDecExpr>("__init__", std::move(params), std::move(body), true);
    }


       std::vector<std::unique_ptr<Expr>> methods;      // Every method from every access section

    while (Current().Type != TokenType::Semicolon)   // Read access sections until the class closes
    {
        if (Current().Type == TokenType::EndOfFile)
        {
            throw std::runtime_error("Non closed scope: class " + name + " is missing ';'");
        }

        if (Current().Type != TokenType::Public && Current().Type != TokenType::Private && Current().Type != TokenType::Protected)
        {
            throw std::runtime_error("Methods must be inside a public, private or protected section");
        }

        TokenType access = Advance().Type; // public, private or protected

        if (Current().Type != TokenType::Colon)
        {
            throw std::runtime_error("An access section must be followed by ':'");
        }

        Advance(); // Move past the section's ':'

        while (Current().Type != TokenType::Semicolon) // Read methods until the section closes
        {
            if (Current().Type == TokenType::EndOfFile)
            {
                throw std::runtime_error("Non closed scope: an access section in " + name + " is missing ';'");
            }

            methods.push_back(ParseMethod(access));
        }

        Advance(); // Move past the section's ';'
    }

    Advance(); // Move past the class's ';'

    return std::make_unique<ClassDecExpr>(name, std::move(constructor), std::move(methods));
}

// Follows grammer rule   <ConstructorStatement> -> <AccessModifier> <VariableType> self.Identifier = <Expression>

std::unique_ptr<Expr> Parser::ParseAttributeDec(){
    TokenType access = Advance().Type; // public, private or protected
    TokenType varType = Advance().Type; // variable types

    Token selfToken = Advance();
    if (selfToken.Value != "self")
    {
        throw std::runtime_error("Attributes must be declared on self, not " + selfToken.Value);
    }

    Advance(); // Move past the '.'

    Token nameToken = Advance(); // The attribute name
    std::string name = nameToken.Value;

    Advance(); // Move past the '='

    std::unique_ptr<Expr> value = ParseExpression();

    return std::make_unique<AttributeDecExpr>(access, varType, name, std::move(value));
}