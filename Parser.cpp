#include "Parser.h"

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

Token Parser::Peek(size_t pos = 1){
    if (position + pos >= (int)tokens.size()) // Stops reading before the end of the endoffile token
    {
        return tokens.back();
    }

    return tokens[position + pos];
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

    if (token.Type == TokenType::Cake){
        Advance();
        return std::make_unique<BooleanExpr>(false);
    } 

    if (token.Type == TokenType::Real){
        Advance();
        return std::make_unique<BooleanExpr>(true);
    }

    if (token.Type == TokenType::Identifier && Peek().Type == TokenType::Arrow)
    {
        return ParseAttributeReference(false);
    }

    if (token.Type == TokenType::Identifier)    //Check if the token is an identifier
    {
        Token next_token = Advance();  // Advance position
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
            throw std::runtime_error("how did we get here: "); // TODO: maybe implement print with this?
        }
                
        // If its not an arrow or a dot function then its just a normal variable/parameter
        // Check if its declared
        Expr* identifier = currentScope->LookUp(name)->Expression;
        if (!dynamic_cast<VarDecExpr*>(identifier) && !dynamic_cast<ParameterExpr*>(identifier))
            throw std::runtime_error("Undeclared Variable: " + name);

        return std::make_unique<IdentifierExpr>(name); // A variable name
    }

    if (token.Type == TokenType::LeftParen)
    {
        Advance();      // Advance to next position
        std::unique_ptr<Expr> inner = ParseBooleanOperator();    // Parse the expression with the brackets
        Advance();  // Adavnce out of the brackets
        return inner;   // Return that expression of the brackets
    }

    currentScope->PrintSymbolTable();
    throw std::runtime_error("Syntax error: unexpected token " + Current().Value);
}

// CODE ADDED FROM MARTIN
std::unique_ptr<Expr> Parser::ParseBooleanOperator(){
    std::unique_ptr<Expr> left = ParseComparisonOperator();

    while(Current().Type == TokenType::Dna || Current().Type == TokenType::Ro)
    {
        TokenType op = Advance().Type;
        std::unique_ptr<Expr> right = ParseComparisonOperator();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }

    return left;
}

std::unique_ptr<Expr> Parser::ParseComparisonOperator(){
    std::unique_ptr<Expr> left = ParseExpression();

    while (
        Current().Type == TokenType::Equalto || 
        Current().Type == TokenType::Equallessthan || 
        Current().Type == TokenType::Istotallydefinitelynotequalto ||
        Current().Type == TokenType::Equalmorethan || 
        Current().Type == TokenType::Lessthan || 
        Current().Type == TokenType::Morethan
    ) {
        TokenType op = Advance().Type;
        std::unique_ptr<Expr> right = ParseExpression();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }
    
    return left;
}
// END CODE ADDED FROM MARTIN

// Creating the parse tree
// This is global scope
std::unique_ptr<ScopeExpr> Parser::ParseProgram(){
    while (Current().Type != TokenType::EndOfFile)
    {
        GlobalScope->Name = "Global Scope";
        GlobalScope->Body.push_back(ParseStatement());   // changed from ParseExpression() so we can use statements too
    }
    return std::move(GlobalScope);
}

// Parse each statement/line of code in the program
std::unique_ptr<Expr> Parser::ParseStatement(){

    TokenType current = Current().Type;     // Gets the current token type 

    if (
        current == TokenType::Vnum || 
        current == TokenType::Vwords || 
        current == TokenType::Vboolean || 
        current == TokenType::Vletter || 
        current == TokenType::VCharacter || 
        current == TokenType::VDigit
    )  // If the token type is a type identifier, call variable declaration
    {
        return ParseVarDec();
    }

    if (current == TokenType::Forwhencake)
    {
        return ParseIteration();
    }

    if (current == TokenType::Handbrake)      // break statement
    {
        return ParseBreak();
    }

    // Pretty big change !!!!!!!!!!!!!!
    if (current == TokenType::IguessIf)
    {
        return ParseConditionalStatementStructure();
    }


    if (current == TokenType::Function)   // Function declaration
    {
        return ParseFunctionDec(SymbolKind::Function);
    }

    if (current == TokenType::Leave)      // Return statement
    {
        return ParseReturn();
    }

    // Attribute declaration
    if ((
        current == TokenType::Public || 
        current == TokenType::Private || 
        current == TokenType::Protected) 
    )   
    {
        return ParseAttributeDec();
    }

    // Make it so that only identifier starts and ends with 'C' is a class expression
    if (
        current == TokenType::Identifier && 
        Current().Value[0] == 'C' && 
        Current().Value.back() == 'C'
    ) 
    {
        if (Peek().Type == TokenType::Colon) return ParseClassDec();
        if (Peek().Type == TokenType::Identifier) return ParseObjectDec();
    }

    std::unique_ptr<Expr> expr = ParseExpression(); // Could be an expression, or the target of an assignment
    if (Current().Type == TokenType::Equals) // It was a target, so this is a reassignment
    {
        if (dynamic_cast<IdentifierExpr*>(expr.get()) == nullptr &&
            dynamic_cast<AttReferenceExpr*>(expr.get()) == nullptr)
        {
            throw std::runtime_error("You cannot assign to that");
        }
        Advance(); // Move past the '='

        std::unique_ptr<Expr> value = ParseExpression(); // The new value
        return std::make_unique<AssignExpr>(std::move(expr), std::move(value));
    }

    return expr;

}

// Bassically a helper function to make ParseVarDec() shorter.
std::unique_ptr<Expr> Parser::ParseGetValue(TokenType type){
    if (type == TokenType::Vwords){
        Token lit = Advance();  // Gets the literal and moves position
        return std::make_unique<WordExpr>(lit.Value);  // Saves this value 
    }

    if (type == TokenType::Vletter)
    {
        Token lit = Advance();          // Gets the literal and moves position
        return std::make_unique<LetterExpr>(lit.Value);    // Saves this value 
    }

    if (type == TokenType::Vboolean)
    {
        Token lit = Advance();          // Gets the literal and moves position
        bool boolValue = NULL;
        if(lit.Type == TokenType::Real){
            boolValue = true;
        }
        else if(lit.Type == TokenType::Cake){
            boolValue = false;
        }

        return std::make_unique<BooleanExpr>(boolValue);    // Saves this value 
    }

    if (type == TokenType::VCharacter){
        Token lit = Advance();          // Gets the literal and moves position

        if(lit.Value.length() != 1){
            throw std::runtime_error("Should be length 1");  // As per the production rules
        }
        return std::make_unique<CharacterExpr>(lit.Value);
    }

    if (type == TokenType::VDigit){
        Token lit = Advance();          // Gets the literal and moves position
        
        if(lit.Type != TokenType::Number || lit.Value.length() != 1)        // Digit must be a number of length 1 as per the production rules
        {
            throw std::runtime_error("Should be length 1 and a number gangalang");
        }
        return std::make_unique<DigitExpr>(lit.Value);
    }

    if(type == TokenType::Vnum){
        return ParseExpression();      // Vnum can be a whole expression or just a value which is covered in parse expression
    } 

    throw std::runtime_error("Thats not a type lil bro lock in");   // Throws an error if its not a valid type
    
}

std::unique_ptr<VarDecExpr> Parser::CreateAndRegisterVarDec (
    TokenType varType, 
    std::string varName, 
    std::unique_ptr<Expr> nameExpr, 
    SymbolKind kind, 
    ScopeExpr* targetScope, 
    TokenType access
) {
    Advance();                                  // Move pass '=' 
    std::unique_ptr<Expr> varValue = ParseGetValue(varType);    // Get var type Expression

    // Create expression and make new entry to symbol table
    std::unique_ptr<VarDecExpr> newVarDecExpr = std::make_unique<VarDecExpr>(varType, std::move(nameExpr), std::move(varValue), access);
    std::unique_ptr<SymbolEntry> newEntry = std::make_unique<SymbolEntry>(varName, varType, kind, newVarDecExpr.get(), access);

    
    // Adding to the current scope syntax table or the accesstor class
    targetScope->Insert(varName, std::move(newEntry));

    return newVarDecExpr;       // returns the expression created
}



// Follows grammer rule     <VariableDeclaration> ->	<VariableType> Identifier = <Expression>
std::unique_ptr<VarDecExpr> Parser::ParseVarDec(){
    TokenType varType = Advance().Type;         // Saves the type

    // Create the expression
    std::string varName = Advance().Value;
    std::unique_ptr<IdentifierExpr> Name = std::make_unique<IdentifierExpr>(varName);

    return CreateAndRegisterVarDec(varType, varName, std::move(Name), SymbolKind::Variable, currentScope);       // returns the expression created
}

 // List -> '"' Identifier '|' <ElementList> '"'
 //<ElementList>	-> 	<Element> || <Element> ‘|’ <ElementList>
std::unique_ptr<ListExpr> Parser::ParseList(){
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
        if (Current().Type == TokenType::Quote && Peek().Type == TokenType::AnotherOne)
        {
            elements.push_back(ParseList());
        }
        else if (Peek().Type == TokenType::Quote && Peek(2).Type != TokenType::AnotherOne)
            break;
    }

    Advance();   // move past "


    std::unique_ptr<ListExpr> newListDecExpr = std::make_unique<ListExpr>(name, std::move(elements));

    std::unique_ptr<SymbolEntry> newEntry = std::make_unique<SymbolEntry>
    (
        name,
        TokenType::List,
        SymbolKind::Variable,
        newListDecExpr.get()
    );
    currentScope->Insert(name, std::move(newEntry));

    return newListDecExpr;       // returns the expression created

}

// Follows grammer rule   <ParameterList> -> <Parameter> | <Parameter> '|' <ParameterList> | NOTHING
std::vector<std::unique_ptr<ParameterExpr>> Parser::ParseParams(){
    std::vector<std::unique_ptr<ParameterExpr>> params;

    while (Current().Type != TokenType::Colon)   // Read parameters until the scope opens
    {
        if (Current().Type == TokenType::EndOfFile)
        {
            throw std::runtime_error("Non closed scope: missing ':'");
        }

        TokenType paramType = Advance().Type;       // parameter type
        std::string paramName = Advance().Value;    // parameter name

        std::unique_ptr<ParameterExpr> newParaExpr = std::make_unique<ParameterExpr>(paramName, paramType);

        std::unique_ptr<SymbolEntry> newEntry =  std::make_unique<SymbolEntry>
            (paramName, paramType, SymbolKind::Parameter, newParaExpr.get());
        currentScope->Insert(paramName, std::move(newEntry));

        params.push_back(std::move(newParaExpr));

        if (Current().Type == TokenType::Separator)   // '|' between parameters
        {
            Advance();
        }
    }

    return params;
}

// Reads a whole scope from ':' <StatementList> ';'
std::vector<std::unique_ptr<Expr>> Parser::ParseBlock(){

    if (Advance().Type != TokenType::Colon)   // check if the scope begins with ':'
        throw std::runtime_error("Non closed scope: missing ':'");

    std::vector<std::unique_ptr<Expr>> body;

    while (Current().Type != TokenType::Semicolon)   // Read statements until the scope closes
    {
        if (Current().Type == TokenType::EndOfFile)
        {
            throw std::runtime_error("Non closed scope: missing ';'");
        }

        body.push_back(ParseStatement());
    }

    Advance();      // Move past the ';'

    return body;
}

std::unique_ptr<IterationExpr> Parser::ParseIteration(){
    Advance(); // Move past 'forwhencake'
    std::vector<std::unique_ptr<Expr>> body = ParseBlock();
    return std::make_unique<IterationExpr>(std::move(body));
}

std::unique_ptr<HandbrakeExpr> Parser::ParseBreak(){
    Advance();      // Move past handbreak
    return std::make_unique<HandbrakeExpr>();
}


std::unique_ptr<ConditionalStatementExpr> Parser::ParseConditionalStatement(){
    TokenType type = Current().Type;
    Advance(); // Move pass ':' to condition line

    std::unique_ptr<Expr> condition = nullptr;
    if (type != TokenType::Guessnot) {
        condition = ParseBooleanOperator(); // generate condition

        if (
            !dynamic_cast<BinaryExpr*>(condition.get()) &&
            !dynamic_cast<BooleanExpr*>(condition.get())
        )
            throw std::runtime_error("Invalid Condition, exprect structure <variable> <operator> <variable> or boolean value");
    }

    std::vector<std::unique_ptr<Expr>> body = ParseBlock();

    return std::make_unique<ConditionalStatementExpr>(type, std::move(condition), std::move(body));
}

std::unique_ptr<ConditionalStatementStructExpr> Parser::ParseConditionalStatementStructure() {
    std::vector<std::unique_ptr<ConditionalStatementExpr>> aConditionStatement;

    if (Current().Type == TokenType::IguessIf)
        aConditionStatement.push_back(ParseConditionalStatement());

    while (Current().Type == TokenType::Guessthis)
        aConditionStatement.push_back(ParseConditionalStatement());

    if (Current().Type == TokenType::Guessnot)
        aConditionStatement.push_back(ParseConditionalStatement());

    return std::make_unique<ConditionalStatementStructExpr>(std::move(aConditionStatement));
}

// Follows grammer rule   <FunctionDeclaration> -> FUNCTION Identifier <ParameterList>: <StatementList>;
std::unique_ptr<FunctionDecExpr> Parser::ParseFunctionDec(SymbolKind symK, TokenType access){
    Advance();                          // Move past FUNCTION
    std::string name = Advance().Value; // Get the function name
    bool isMethod = (symK == SymbolKind::Method) ? true : false;
    
    std::unique_ptr<FunctionDecExpr> newFuncDecExpr = std::make_unique<FunctionDecExpr>(name, isMethod, currentScope, access);

    // Change the current scope so that stuff in the block is added to the correct syntax table
    currentScope = newFuncDecExpr.get();

    // Parsing parameters and the statements inside the function 
    std::vector<std::unique_ptr<ParameterExpr>> params = ParseParams();
    std::vector<std::unique_ptr<Expr>> body = ParseBlock();

    std::unique_ptr<SymbolEntry> newEntry =  std::make_unique<SymbolEntry>
        (name, TokenType::Function, symK, newFuncDecExpr.get(), access);

    // Go back to previous scope after done parsing body and add function to syntax table
    currentScope = currentScope->ParentScope;
    currentScope->Insert(name, std::move(newEntry));

    // Update function expression attribute
    newFuncDecExpr->Body = std::move(body);
    newFuncDecExpr->Param = std::move(params);

    return newFuncDecExpr;
}

// Follows grammer rule   <ReturnStatement> -> leave <Expression>
std::unique_ptr<ReturnExpr> Parser::ParseReturn(){
    Advance();      // Move past leave
    return std::make_unique<ReturnExpr>(ParseExpression());
}

// Reads exactly as many arguments as were declared, with a '|' between each.
std::vector<std::unique_ptr<Expr>> Parser::ParseArgs(int expected, std::string name){
    std::vector<std::unique_ptr<Expr>> args;

    for (int i = 0; i < expected; i++)
    {
        if (i > 0)
        {
            if (Current().Type != TokenType::Separator)
            {
                throw std::runtime_error(name + " did not get enough arguments");
            }

            Advance(); // Move past the '|'
        }

        args.push_back(ParseExpression());
    }

    return args;
}

// Follows grammer rule   <FunctionCall> -> f <FunctionName> <ArgumentList>
std::unique_ptr<FunctionCallExpr> Parser::ParseFunctionCall(ClassDecExpr* primaryClassDecExpr){
    
    // Move past the 'f' and Get the function name
    Advance();
    std::string funcName = Advance().Value; 

    // Get the scope this function was called in. 
    // We need it because we can call a class method in the scope of a function as long as that function is in a class dec scope.
    ScopeExpr* activeScope =  currentScope;
    if (primaryClassDecExpr) activeScope = primaryClassDecExpr;

    // Check current scope symbol table to see if function exists, and to returns the correct expression
    SymbolEntry* funcSymbol = activeScope->LookUp(funcName);
    if (funcSymbol->SymK != SymbolKind::Function && funcSymbol->SymK != SymbolKind::Method) 
        throw std::runtime_error("Cannot find function/method " + funcName);
    
    FunctionDecExpr* funcDecExpr = dynamic_cast<FunctionDecExpr*>(funcSymbol->Expression);
    if (!funcDecExpr)   
        throw std::runtime_error("Symbol talbe store wrong expr for function: " + funcName);

    std::vector<ParameterExpr*> expected = activeScope->GetFunctionPara(funcName);

    std::vector<std::unique_ptr<Expr>> args = ParseArgs(expected.size(), funcName);

    return std::make_unique<FunctionCallExpr>(funcDecExpr, std::move(args));
}


std::unique_ptr<AttReferenceExpr> Parser::ParseAttributeReference(bool declaration, TokenType access)
{
    // Get the Callee/Primary name can create an Id expression. 
    Token token = Current();
    std::unique_ptr<IdentifierExpr> primaryName = std::make_unique<IdentifierExpr>(token.Value);
    Advance();      // Move pass the caller
    Advance();      // Move pass the arrow

    // Only objects are allowed to use ->
    SymbolEntry* primarySymbol = currentScope->LookUp(token.Value);
    if (primarySymbol->SymK != SymbolKind::Object) {
        throw std::runtime_error("Variable does not have an arrow expression: " + token.Value);
    }

    // Check if its Vnum selfish -> ATTRIBUTE. Because both declaration and call use -> 
    if (declaration) {
        std::unique_ptr<IdentifierExpr> attributeName = std::make_unique<IdentifierExpr>(Advance().Value);
        return std::make_unique<AttReferenceExpr>(std::move(primaryName), std::move(attributeName), access);
    }


    // Check if we're inside the scope of a class
    ClassDecExpr* primaryClassDecExpr;
    ClassDecExpr* potentialClassExpr = dynamic_cast<ClassDecExpr*>(primarySymbol->Expression);
    if (token.Value == "selfish" && potentialClassExpr) {       // Every class dec expr will have selfish in the symbol table points it the expr  
        primaryClassDecExpr = potentialClassExpr;
    } else {
        ObjectDecExpr* primaryExpr = dynamic_cast<ObjectDecExpr*>(primarySymbol->Expression);
        primaryClassDecExpr = primaryExpr->ClassDefExpr;
    }


    // Use the arrow function to call function  selfish -> f FUNCTION
    if (Current().Type == TokenType::Call) {
        SymbolEntry* methodSymbol = primaryClassDecExpr->LookUpLocal(Peek().Value);

        // Check if its a public method when called outside a class dec
        if (!currentScope->GetParentClass() && methodSymbol->Access != TokenType::Public) 
            throw std::runtime_error(primarySymbol->Name + " has method: " + methodSymbol->Name + " but cant be called outside class dec");
        if (methodSymbol->SymK != SymbolKind::Method)
            throw std::runtime_error(primarySymbol->Name + " has no method: " + methodSymbol->Name);

        // Just normal function call expr
        std::unique_ptr<FunctionCallExpr> methodCallExpr = ParseFunctionCall(primaryClassDecExpr);
        return std::make_unique<AttReferenceExpr>(std::move(primaryName), std::move(methodCallExpr), methodSymbol->Access);
    }

    // Check for selfish -> f ATTRIBUTE
    SymbolEntry* identifierSymbol = primaryClassDecExpr->LookUpLocal(Advance().Value);
    if (identifierSymbol->SymK == SymbolKind::Attribute)
    {
        // Check if its a public method when called outside a class dec
        if (!currentScope->GetParentClass() && identifierSymbol->Access != TokenType::Public) 
            throw std::runtime_error(primarySymbol->Name + "has attribute: " + identifierSymbol->Name + " but cant be called outside class dec");

        // Just a normal id expr
        std::unique_ptr<IdentifierExpr> attributeName = std::make_unique<IdentifierExpr>(identifierSymbol->Name);
        return std::make_unique<AttReferenceExpr>(std::move(primaryName), std::move(attributeName), identifierSymbol->Access);
    }
    
    throw std::runtime_error(primarySymbol->Name + "object does not have attribute/function: " + identifierSymbol->Name);
}

// Basically the same as normal attribute, just change the IsAttrute and access field.
std::unique_ptr<VarDecExpr> Parser::ParseAttributeDec(){ // Parses 'public Vnum self.x = ...'
    TokenType access = Advance().Type;          // public, private or protected
    TokenType varType = Advance().Type;         // Saves the type
    std::string varName = Peek(2).Value;        // Skipping over 'selfish' and -> to get the name 

    // Check if this attribute is declared inside a class
    if (!currentScope->GetParentClass()) {
        throw std::runtime_error("Cant find parent class to declare: " + varName);
    }

    // Getting the expression for selfish -> ATTRIBUTE_NAME
    std::unique_ptr<Expr> Name = ParseAttributeReference(true, access);   

    return CreateAndRegisterVarDec (varType, varName, std::move(Name), SymbolKind::Attribute, currentScope->GetParentClass(), access);
}

// Follows grammer rule   <Methods> -> <FunctionDeclaration> | <FunctionDeclaration> <Methods>
std::unique_ptr<ClassDecExpr> Parser::ParseClassDec(){ // Parses class declarations

    std::string className = Advance().Value;    // Get the class name
    Advance(); // Move past the ':'

    std::unique_ptr<ClassDecExpr> newClassDecExpr = std::make_unique<ClassDecExpr>(className, currentScope);

    currentScope = newClassDecExpr.get();   // Change scope into the class to parse its body

    // Looping through the class body
    while (Current().Type != TokenType::Semicolon) {  // Read access sections until the class closes
        TokenType currentType = Current().Type;
        if (currentType == TokenType::EndOfFile) 
        {
            throw std::runtime_error("Non closed scope: class " + className + " is missing ';'");
        }

        // Parsing the constructor.
        if (currentType == TokenType::Function && Peek().Type == TokenType::SELF)
        {
            std::unique_ptr<FunctionDecExpr> classMethodExpr = ParseFunctionDec(SymbolKind::Method, TokenType::Public);
            newClassDecExpr->Body.push_back(std::move(classMethodExpr));
            continue;
        }

        // Follows grammer rule   <Methods> -> <FunctionDeclaration> | <FunctionDeclaration> <Methods>
        // Parsing each of the scope for access modifier
        if (
            (currentType == TokenType::Public || 
            currentType == TokenType::Private ||
            currentType == TokenType::Protected) &&
            Peek().Type == TokenType::Colon
        ) {
            TokenType access = Advance().Type;  // Go pass Public/Private/Protected
            Advance();  // Go pass ':'
            while (Current().Type != TokenType::Semicolon) { // Read methods until the section closes
                
                if (Current().Type == TokenType::EndOfFile) {
                    throw std::runtime_error("Non closed scope: an access section in " + className + " is missing ';'");
                }
                std::unique_ptr<FunctionDecExpr> classMethodExpr = ParseFunctionDec(SymbolKind::Method, access);
                newClassDecExpr->Body.push_back(std::move(classMethodExpr));
            }
            Advance(); // Move past the section's ';'
            continue;
        }
    }
    Advance(); // Move past the class's ';'

    // Add the new class to the symbol table
    std::unique_ptr<SymbolEntry> newEntry =  std::make_unique<SymbolEntry>
        (className, TokenType::Custom, SymbolKind::Class, newClassDecExpr.get());

    currentScope = currentScope->ParentScope;

    currentScope->Insert(className, std::move(newEntry));

    return newClassDecExpr;
}


std::unique_ptr<ObjectDecExpr> Parser::ParseObjectDec()// Parses 'CName obj = CName args'
{
    std::string className = Advance().Value; // The class name on the left
    std::string objName = Advance().Value; // obejct name

    // Class was not declared
    SymbolEntry* classSymbol = currentScope->LookUp(className);
    if (classSymbol->SymK != SymbolKind::Class) 
        throw std::runtime_error("Cannot Recognize Class: " + className);
    
    ClassDecExpr* classExpr = dynamic_cast<ClassDecExpr*>(classSymbol->Expression);
    if (!classExpr)
        throw std::runtime_error("Symbol talbe store wrong expr for class: " + className);

    Advance(); // Move past the '='

    Token secondToken = Advance();
    if (secondToken.Value != className)
        throw std::runtime_error("Object " + objName + " was declared as " + className + " but built as " + secondToken.Value);


    // How many arguments the constructor takes
    std::vector<ParameterExpr*> expected = classExpr->GetFunctionPara(); 
    std::vector<std::unique_ptr<Expr>> args = ParseArgs(expected.size(), "Class " + className);

    // Get the class expression and create new object expression
    std::unique_ptr<ObjectDecExpr> newObjDecExpr = 
        std::make_unique<ObjectDecExpr>(classExpr, objName, std::move(args));

    // Adding to the symbol table
    std::unique_ptr<SymbolEntry> newEntry = make_unique<SymbolEntry>
        (objName, TokenType::Custom, SymbolKind::Object, newObjDecExpr.get());

    currentScope->Insert(objName, move(newEntry));

    return newObjDecExpr;
}; 
