#include "Parser.h"

Parser::Parser(std::vector<Token> tokens){
    this->tokens = tokens;    // Initialise tokens
    this->position = 0;       // Initialise position
    this->inWhile = false;
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

        if (token.Type == TokenType::Word)
    {
        Advance();
        return std::make_unique<WordExpr>(token.Value);
    }

    if (token.Type == TokenType::Letter)
    {
        Advance();
        return std::make_unique<LetterExpr>(token.Value);
    }

    if (token.Type == TokenType::Cake){
        Advance();
        return std::make_unique<BooleanExpr>(false);
    } 

    if (token.Type == TokenType::Real){
        Advance();
        return std::make_unique<BooleanExpr>(true);
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
                

        if (Current().Type == TokenType::Arrow){ // Ford1 -> something
            Advance(); // Move past the '->'

            if (!currentScope->Contain(name))   // The object was never declared
            {
                throw std::runtime_error("Cannot Recognize " + name);
            }

            // Get object expression
            Expr* symbolExpr = currentScope->LookUp(name)->Expression;   
            ObjectDecExpr* objExpr = dynamic_cast<ObjectDecExpr*>(symbolExpr);

            // Check if the variable is an object. 
            // Or if we're currently inside class declaration, check the selfish keyword points to its class
            if (!objExpr && !dynamic_cast<ClassDecExpr*>(symbolExpr)) 
            {
                throw std::runtime_error("Variable does not have an arrow expression: " + next_token.Value);
            }


            // Seperate check for object and selfish keyword to get class declaration expression
            ClassDecExpr* classExpr;
            if (name != "selfish") {
                classExpr = objExpr->ClassDefExpr;
            } else {
                classExpr = dynamic_cast<ClassDecExpr*>(symbolExpr);
            }

            // See what object called the '->'
            std::unique_ptr<Expr> callee = make_unique<IdentifierExpr>(next_token.Value);

            if (Current().Type == TokenType::Call) // Check if its method call Ford1 -> f METHOD args
            {
                return ParseFunctionCall(classExpr, std::move(callee));
            }

            // If its not method call then its an attribute
            Token attrToken = Advance(); 

            // Check if it is declared
            SymbolEntry* attSymbol = classExpr->LookUpLocal(attrToken.Value);
            if (attSymbol->SymK != SymbolKind::Variable) {
                throw std::runtime_error("Not an attribute: " + next_token.Value);
            }
            return std::make_unique<IdentifierExpr>(attSymbol->Name, std::move(callee));
        }

        // If its not an arrow or a dot function then its just a normal variable/parameter
        // Check if its declared
        Expr* identifier = currentScope->LookUp(name)->Expression;
        if (!dynamic_cast<VarDecExpr*>(identifier) && !dynamic_cast<ParameterExpr*>(identifier)){
            throw std::runtime_error("Undeclared Variable: " + name);
        }
        return std::make_unique<IdentifierExpr>(name); // A variable name
    }

    if (token.Type == TokenType::LeftParen)
    {
        Advance();      // Advance to next position
        std::unique_ptr<Expr> inner = ParseBooleanOperator();    // Parse the expression with the brackets
        Advance();  // Adavnce out of the brackets
        return inner;   // Return that expression of the brackets
    }

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
        return ParseVarDec(false, TokenType::Public);
    }

    if (current == TokenType::Forwhencake)
    {   
        return ParseIteration();
    }

    if (current == TokenType::Handbrake)      // break statement
    {   
        if(inWhile == true){
            return ParseBreak();
        }
        else{
            throw std::runtime_error("Invalid Syntax: Handbrake");
        }
    }

    // Pretty big change !!!!!!!!!!!!!!
    if (current == TokenType::IguessIf)
    {
        return ParseConditionalStatementStructure();
    }


    if (current == TokenType::Function)   // Function declaration
    {
        return ParseFunctionDec();
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
        if (dynamic_cast<Expr*>(expr.get()) == nullptr)
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
    // ParseVarDec checks if right type
    if (Current().Type == TokenType::Call || Peek().Type == TokenType::Arrow)
    {
        return ParseExpression();
    }

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

// Follows grammer rule     <VariableDeclaration> ->	<VariableType> Identifier = <Expression>
std::unique_ptr<VarDecExpr> Parser::ParseVarDec(bool isAttribute, TokenType access){
    TokenType varType = Advance().Type;    // Saves the type and name of declared variable             
    
    // Enforcing class attribute declaration syntax
    if (isAttribute){
        if (Advance().Value != "selfish")
            throw std::runtime_error("Attributes must be declared on selfish.");
        
        if (Advance().Type != TokenType::Arrow)
            throw std::runtime_error("Attributes must be use arrow");
    }

    // Get the assigned value and Move pass '=' 
    std::string varName = Advance().Value;
    Advance();

    // The function above
    std::unique_ptr<Expr> varValue = ParseGetValue(varType); 
    // Value must be same type as variable declared
    CheckType(varType, varValue.get(), "Variable '" + varName + "'");

    // Create expression and make new entry to symbol table
    std::unique_ptr<VarDecExpr> newVarDecExpr = std::make_unique<VarDecExpr>(varType, varName, std::move(varValue), access, isAttribute);
    std::unique_ptr<SymbolEntry> newEntry = std::make_unique<SymbolEntry>(
        varName,
        varType,
        SymbolKind::Variable,
        newVarDecExpr.get()
    );

    // Deciding whether to add to the current scope syntax table or the accesstor class
    if (isAttribute){
        if (ClassDecExpr* classExpr =  currentScope->GetParentClass())
            classExpr->Insert(varName, std::move(newEntry));
        else 
            throw std::runtime_error("Cant find parent class to declare: " + varName);
    } else {
        currentScope->Insert(varName, std::move(newEntry));
    }

    return newVarDecExpr;       // returns the expression created
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

    if (Current().Type != TokenType::Quote)
    {
        throw std::runtime_error("Expected closing \" for list '" + name + "' but found: " + Current().Value);
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
        (
            paramName,
            paramType,
            SymbolKind::Parameter,
            newParaExpr.get()
        );
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
    bool childWhile = false;
    if(inWhile == true){
        childWhile = true;
    }
    else{
        inWhile = true;
    }
    
    std::unique_ptr<IterationExpr> newIterationExpr = std::make_unique<IterationExpr>(currentScope);
    
    currentScope = newIterationExpr.get();

    newIterationExpr->Body = ParseBlock();

    currentScope = newIterationExpr->ParentScope;
    
    if(childWhile == false){
        inWhile = false;
    }
    return std::move(newIterationExpr);
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

        if (!dynamic_cast<BinaryExpr*>(condition.get()) && !dynamic_cast<BooleanExpr*>(condition.get())){
            if(dynamic_cast<IdentifierExpr*>(condition.get())){
                if(currentScope->LookUp(tokens[position-2].Value)->Type != TokenType::Vboolean){
                    throw std::runtime_error("Invalid Condition, expect structure <variable> <operator> <variable> or boolean value");
                }
            }
            else{
                throw std::runtime_error("Invalid Condition, expect structure <variable> <operator> <variable> or boolean value");
            }
        }
    }

    //create new scope
    std::unique_ptr<ConditionalStatementExpr> newCondStateExpr = std::make_unique<ConditionalStatementExpr>(type, std::move(condition), currentScope);
    
    currentScope = newCondStateExpr.get();

    // generate body
    newCondStateExpr->Body = ParseBlock();

    // reset to parent scope after finsih body part
    currentScope = currentScope->ParentScope;

    return std::move(newCondStateExpr);
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

// Follows grammer rule <FunctionDeclaration> -> FUNCTION <ReturnType> Identifier <ParameterList>: <StatementList>;
// <ReturnType> -> <VariableType> | NOTHING
std::unique_ptr<FunctionDecExpr> Parser::ParseFunctionDec(){
    Advance();                          // Move past FUNCTION

    // Get the return type if there is one
    TokenType returnType = TokenType::Nothing;
    TokenType current = Current().Type;
    if (
        current == TokenType::Vnum || 
        current == TokenType::Vwords || 
        current == TokenType::Vboolean || 
        current == TokenType::Vletter || 
        current == TokenType::VCharacter || 
        current == TokenType::VDigit
    ) {
        returnType = Advance().Type;
    }

    std::string name = Advance().Value; // Get the function name
    
    std::unique_ptr<FunctionDecExpr> newFuncDecExpr = std::make_unique<FunctionDecExpr>(name, currentScope);
    newFuncDecExpr->ReturnType = returnType;

    // Change the current scope so that stuff in the block is added to the correct syntax table
    currentScope = newFuncDecExpr.get();

    // Parsing parameters and the statements inside the function 
    std::vector<std::unique_ptr<ParameterExpr>> params = ParseParams();
    std::vector<std::unique_ptr<Expr>> body = ParseBlock();

    // The return type is saved in the symbol table so function calls can be type checked
    std::unique_ptr<SymbolEntry> newEntry =  std::make_unique<SymbolEntry>
        (name, returnType, SymbolKind::Function, newFuncDecExpr.get());

    // Go back to previous scope after done parsing body and add function to syntax table
    currentScope = currentScope->ParentScope;
    currentScope->Insert(name, std::move(newEntry));

    // Update function expression attribute
    newFuncDecExpr->Body = std::move(body);
    newFuncDecExpr->Param = std::move(params);

    return std::move(newFuncDecExpr);
}

// function for checking the scope is whether or not a FunctionDecExpr
FunctionDecExpr* Parser::CheckParentScope(ScopeExpr* curr){
    if (curr == nullptr){
        return nullptr;
    }
    FunctionDecExpr* returnValue = dynamic_cast<FunctionDecExpr*>(curr);
    
    if(returnValue != nullptr){
        return returnValue;
    }

    return CheckParentScope(curr->ParentScope);
}

// Follows grammer rule   <ReturnStatement> -> leave <Expression>
// must match return type of the function
std::unique_ptr<ReturnExpr> Parser::ParseReturn(){
    Advance();      // Move past leave

    FunctionDecExpr* function = CheckParentScope(currentScope);
    if (!function)
    {   
        throw std::runtime_error("leave can only be used inside a FUNCTION");
    }

    if (function->ReturnType == TokenType::Nothing)
    {
        throw std::runtime_error("Function " + function->Name + " has no return type so it can't leave anything");
    }

    std::unique_ptr<Expr> value = ParseExpression();
    CheckType(function->ReturnType, value.get(), "leave in function " + function->Name);

    return std::make_unique<ReturnExpr>(std::move(value));
}

// Turns a type token into its name for error messages
static std::string TypeName(TokenType type){
    switch (type)
    {
        case TokenType::Vnum:       return "Vnum";
        case TokenType::Vwords:     return "Vwords";
        case TokenType::Vboolean:   return "Vboolean";
        case TokenType::Vletter:    return "Vletter";
        case TokenType::VCharacter: return "VCharacter";
        case TokenType::VDigit:     return "VDigit";
        case TokenType::List:       return "List";
        default:                    return "unknown";
    }
}

// Works out the type of an expression so it can be compared with a parameter type
// Returns TokenType::Custom when the type can't be known yet (function return values, list.at)
TokenType Parser::GetExprType(Expr* expr){
    if (dynamic_cast<NumberExpr*>(expr))    return TokenType::Vnum;
    if (dynamic_cast<WordExpr*>(expr))      return TokenType::Vwords;
    if (dynamic_cast<LetterExpr*>(expr))    return TokenType::Vletter;
    if (dynamic_cast<BooleanExpr*>(expr))   return TokenType::Vboolean;
    if (dynamic_cast<CharacterExpr*>(expr)) return TokenType::VCharacter;
    if (dynamic_cast<DigitExpr*>(expr))     return TokenType::VDigit;
    if (dynamic_cast<ListSizeExpr*>(expr))  return TokenType::Vnum;

    if (BinaryExpr* binary = dynamic_cast<BinaryExpr*>(expr))
    {
        if (
            binary->Operator == TokenType::Add || 
            binary->Operator == TokenType::Sub ||
            binary->Operator == TokenType::Mul || 
            binary->Operator == TokenType::Div || 
            binary->Operator == TokenType::Mod
        ) {
            TokenType left = GetExprType(binary->Left.get());
            TokenType right = GetExprType(binary->Right.get());

            if (left != TokenType::Vnum && left != TokenType::VDigit && left != TokenType::Custom) return left;
            if (right != TokenType::Vnum && right != TokenType::VDigit && right != TokenType::Custom) return right;
            return TokenType::Vnum;
        }

        return TokenType::Vboolean;
    }

    // Return type is saved in the symbol table of the scope it was declared in
    if (FunctionCallExpr* call = dynamic_cast<FunctionCallExpr*>(expr))
    {
        FunctionDecExpr* function = call->FuncDecExpr;
        return function->ParentScope->LookUpLocal(function->Name)->Type;
    }

    if (IdentifierExpr* identifier = dynamic_cast<IdentifierExpr*>(expr))
    {
        // Attribute like speed, look it up in the class
        if (IdentifierExpr* callee = dynamic_cast<IdentifierExpr*>(identifier->Callee.get()))
        {
            Expr* calleeExpr = currentScope->LookUp(callee->Name)->Expression;
            ClassDecExpr* classExpr = dynamic_cast<ClassDecExpr*>(calleeExpr);

            if (ObjectDecExpr* objExpr = dynamic_cast<ObjectDecExpr*>(calleeExpr))
            {
                classExpr = objExpr->ClassDefExpr;
            }

            return classExpr->LookUpLocal(identifier->Name)->Type;
        }

        return currentScope->LookUp(identifier->Name)->Type; // Normal variable or parameter
    }

    return TokenType::Custom; // Function calls and list.at dont have a known type yet
}

// Throws an error if the value is not the right type
void Parser::CheckType(TokenType wanted, Expr* value, std::string what){
    TokenType given = GetExprType(value);

    if (given == TokenType::Nothing)
    {
        throw std::runtime_error(what + " should be " + TypeName(wanted) + " but the function doesnt leave anything");
    }

    bool matches = 
        given == wanted ||
        given == TokenType::Custom ||
        (wanted == TokenType::Vnum && given == TokenType::VDigit);

    // A single digit number can be used as a VDigit
    NumberExpr* number = dynamic_cast<NumberExpr*>(value);
    if (wanted == TokenType::VDigit && number && number->Value.length() == 1)
    {
        matches = true;
    }

    if (!matches)
    {
        throw std::runtime_error(what + " should be " + TypeName(wanted) + " but got " + TypeName(given));
    }
}

// Reads exactly as many arguments as were declared, with a '|' between each.
// Each argument has to match the type of the parameter it lines up with.
std::vector<std::unique_ptr<Expr>> Parser::ParseArgs(std::vector<ParameterExpr*> expected, std::string name){
    std::vector<std::unique_ptr<Expr>> args;

    for (size_t i = 0; i < expected.size(); i++)
    {
        if (i > 0)
        {
            if (Current().Type != TokenType::Separator)
            {
                throw std::runtime_error(name + " did not get enough arguments");
            }

            Advance(); // Move past the '|'
        }

        std::unique_ptr<Expr> arg = ParseExpression();

        // The argument has to match the type of the parameter
        CheckType(expected[i]->Type, arg.get(), name + " argument '" + expected[i]->Name + "'");

        args.push_back(std::move(arg));
    }

    return args;
}

// Follows grammer rule   <FunctionCall> -> f <FunctionName> <ArgumentList>
std::unique_ptr<FunctionCallExpr> Parser::ParseFunctionCall(ClassDecExpr* classExpr, std::unique_ptr<Expr> callee){
    
    // Move past the 'f' and Get the function name
    Advance();      
    std::string funcName = Advance().Value; 

    // Check if this function call is inside a class and  that function is also this class method
    ClassDecExpr* classDecExpr = currentScope->GetParentClass();
    if (
        classDecExpr && 
        classExpr == nullptr && callee == nullptr &&
        !classDecExpr->ParentScope->Contain(funcName) &&
        classDecExpr->ContainLocal(funcName)
    ) {
        throw std::runtime_error("Wrong way to call method " + funcName);
    }
    
    ScopeExpr* activeScope =  currentScope;
    SymbolEntry* funcSymbol;

    // Needed to see if this function is a method is being called inside a class or not
    if (classExpr) {
        activeScope = classExpr;
        funcSymbol = activeScope->LookUpLocal(funcName);
    } else {
        funcSymbol = activeScope->LookUp(funcName);
    }

    FunctionDecExpr* funcDecExpr = dynamic_cast<FunctionDecExpr*>(funcSymbol->Expression);

    if (funcSymbol->SymK != SymbolKind::Function || !funcDecExpr) 
    {
        throw std::runtime_error("Cannot find function/method " + funcName);
    }

    std::vector<ParameterExpr*> expected = activeScope->GetFunctionPara(funcName);

    std::vector<std::unique_ptr<Expr>> args = ParseArgs(expected, "Method " + funcName);

    return std::make_unique<FunctionCallExpr>(funcDecExpr, std::move(args), std::move(callee));
}

// Basically the same as normal attribute, just change the IsAttrute and access field.
std::unique_ptr<VarDecExpr> Parser::ParseAttributeDec(){ // Parses 'public Vnum self.x = ...'
    TokenType access = Advance().Type; // public, private or protected
    std::unique_ptr<VarDecExpr> newAttExpr = ParseVarDec(true, access);
    return std::move(newAttExpr);       // returns the expression created
}

// Follows grammer rule   <Methods> -> <FunctionDeclaration> | <FunctionDeclaration> <Methods>
// Basically the same as normal function, just change the IsMethod and access field.
std::unique_ptr<FunctionDecExpr> Parser::ParseMethod(TokenType access = TokenType::Public) // Parses one FUNCTION inside an access section
{
    std::unique_ptr<FunctionDecExpr> classMethodExpr = ParseFunctionDec();
    classMethodExpr->Access = access;
    classMethodExpr->IsMethod = true;
    return std::move(classMethodExpr);
}


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
            newClassDecExpr->Body.push_back(std::move(ParseMethod()));
            continue;
        }

        // Parsing each of the scope for access modifier
        if (
            (currentType == TokenType::Public || 
            currentType == TokenType::Private ||
            currentType == TokenType::Protected) &&
            Peek().Type == TokenType::Colon
        ) {
            Advance();  // Go pass Public/Private/Protected
            Advance();  // Go pass ':'
            while (Current().Type != TokenType::Semicolon) { // Read methods until the section closes
                
                if (Current().Type == TokenType::EndOfFile) {
                    throw std::runtime_error("Non closed scope: an access section in " + className + " is missing ';'");
                }
                newClassDecExpr->Body.push_back(std::move(ParseMethod(currentType)));
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

    return std::move(newClassDecExpr);
}


std::unique_ptr<ObjectDecExpr> Parser::ParseObjectDec()// Parses 'CName obj = CName args'
{
    std::string className = Advance().Value; // The class name on the left
    std::string objName = Advance().Value; // obejct name

    // Class was not declared
    SymbolEntry* classSymbol = currentScope->LookUp(className);
    if (classSymbol->SymK != SymbolKind::Class) 
    {
        throw std::runtime_error("Cannot Recognize Class: " + className);
    }
    ClassDecExpr* classExpr = dynamic_cast<ClassDecExpr*>(classSymbol->Expression);
    

    Advance(); // Move past the '='

    Token secondToken = Advance();
    if (secondToken.Value != className)
    {
        throw std::runtime_error("Object " + objName + " was declared as " + className + " but built as " + secondToken.Value);
    }


    // How many arguments the constructor takes
    std::vector<ParameterExpr*> expected = classExpr->GetFunctionPara(); 
    std::vector<std::unique_ptr<Expr>> args = ParseArgs(expected, "Class " + className);

    // Get the class expression and create new object expression
    std::unique_ptr<ObjectDecExpr> newObjDecExpr = 
        std::make_unique<ObjectDecExpr>(classExpr, objName, std::move(args));

    // Adding to the symbol table
    std::unique_ptr<SymbolEntry> newEntry = make_unique<SymbolEntry>
        (objName, TokenType::Custom, SymbolKind::Variable, newObjDecExpr.get());

    currentScope->Insert(objName, move(newEntry));

    return newObjDecExpr;
}; 
