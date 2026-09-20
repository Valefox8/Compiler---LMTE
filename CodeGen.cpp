#include "CodeGen.h"
#include <stdexcept>
#include <iostream>

std::string CodeGen::Operator(TokenType op){
    if(op == TokenType::Add){
        return "+";
    }
    if(op == TokenType::Sub){
        return "-";
    }
    if(op == TokenType::Mul){
        return "*";
    }
    if(op == TokenType::Div){
        return "/";
    }
    if(op == TokenType::Mod){
        return "%";
    }
    if(op == TokenType::Dna){
        return "and";
    }
    if(op == TokenType::Ro){
        return "or";
    }
    if(op == TokenType::Equalto){
        return "==";
    }
    if(op == TokenType::Lessthan){
        return "<";
    }
    if(op == TokenType::Morethan){
        return ">";
    }
    if(op == TokenType::Istotallydefinitelynotequalto){
        return "!=";
    }
    if(op == TokenType::Equallessthan){
        return "<=";
    }
    if(op == TokenType::Equalmorethan){
        return ">=";
    }

    throw std::runtime_error("Unknown op");
}

// level represents the amount of \t to add
std::string CodeGen::Iteration(int level, IterationExpr* expr){
    std::cout<< "Iteration Level: " << level << std::endl;
    std::string tabs = std::string(level, '\t');
    std::string changeline = "\n";
    std::string codes = tabs + "while(True):" + changeline;

    for(const auto& codeline : expr->Codeline){
        codes += GenCode(level + 1, codeline.get());
    }

    return codes;
}

std::string CodeGen::ConditionalStatement(int level, ConditionalStatementExpr* expr){
    std::cout<< "CS Level: " << level << std::endl;
    std::string tabs = std::string(level, '\t');
    std::string changeline = "\n";
    
    // check if else elif
    TokenType type = expr->Type;
    std::string condition;
    std::string codes;

    if(type == TokenType::IguessIf){
        //generate condition codes
        condition = GenCode(0, expr->Condition.get());
        codes = tabs + "if " + condition + ":" + changeline;
    }
    else if (type == TokenType::Guessthis)
    {
        //generate condition codes
        condition = GenCode(0, expr->Condition.get());
        codes = tabs + "elif " + condition + ":" + changeline;
    }
    else if (type == TokenType::Guessnot)
    {
        codes = tabs + "else:" + changeline;
    }
    

    for(const auto& codeline : expr->Codeline){
        codes += GenCode(level + 1, codeline.get());
    }

    return codes;
}

std::string CodeGen::ConditionalStatementStruct(int level, ConditionalStatementStructExpr* expr){
    std::string codes = "";

    for(const auto& codeline : expr->ConditionalStatements){
        auto* conditional = dynamic_cast<ConditionalStatementExpr*>(codeline.get());
        codes += ConditionalStatement(level, conditional);
    }
    return codes;
}

std::string CodeGen::GenCode(int level, Expr* expr){

    // Return the number as its text value
    std::string tabs = std::string(level, '\t');

    if (NumberExpr* number = dynamic_cast<NumberExpr*>(expr))
    {
        return number->Value;
    }

    // Return the variable name
    if (IdentifierExpr* identifier = dynamic_cast<IdentifierExpr*>(expr))
    {
        return identifier->Name;
    }

    // Expression, generate both sides and the operator before printing
    if (BinaryExpr* binary = dynamic_cast<BinaryExpr*>(expr))
    {
        std::string left = GenCode(0, binary->Left.get());
        std::string right = GenCode(0, binary->Right.get());
        std::string op = Operator(binary->Operator);

        return tabs + "(" + left + " " + op + " " + right + ")";
    }

    // Word becomes a Python string, wrapped in quotes
    if (WordExpr* word = dynamic_cast<WordExpr*>(expr))
    {
        return "\"" + word->Value + "\"";
    }

    // Letter becomes a Python string, wrapped in quotes
    if (LetterExpr* letter = dynamic_cast<LetterExpr*>(expr))
    {
        return "\"" + letter->Value + "\"";
    }

    // Character becomes a Python string, wrapped in quotes
    if (CharacterExpr* character = dynamic_cast<CharacterExpr*>(expr))
    {
        return "\"" + character->Value + "\"";
    }

    // Digit returns just the value
    if (DigitExpr* digit = dynamic_cast<DigitExpr*>(expr))
    {
        return digit->Value;
    }

    // Boolean 
    if (BooleanExpr* boolean = dynamic_cast<BooleanExpr*>(expr))
    {
        if (boolean->Value == true)  // Checks if its "real"
        {
            return "True";  // returns true if so
        }

        return "False";     // otherwise returns false
    }

    // Variable declaration
    if (VarDecExpr* decl = dynamic_cast<VarDecExpr*>(expr))
    {
        std::string valueText = GenCode(0, decl->Value.get()); // Gets the value 
        return tabs + decl->Name + " = " + valueText + "\n";  // Returns and constructs the assignement with the name, equals sign and value we just got
    }

    if(HandbrakeExpr* handbrake = dynamic_cast<HandbrakeExpr*>(expr)){
        return tabs + "break\n";
    }
    
    if(IterationExpr* iteration = dynamic_cast<IterationExpr*>(expr)){
        return Iteration(level, iteration);
    }

    if(ConditionalStatementStructExpr* conditionalStatementStruct = dynamic_cast<ConditionalStatementStructExpr*>(expr)){
        return ConditionalStatementStruct(level, conditionalStatementStruct);
    }


    // Creating the list
    if(ListExpr* list = dynamic_cast<ListExpr*>(expr)){

        // Grabs the name and the strat of the python list syntax
        std::string result = list->Name + " = [";

        // For each element in the elements vector, create a new python list entry 
        for(size_t i = 0; i < list->Elements.size(); i++){
            if(i > 0){
                result += ", ";
            }

            result += GenCode(0, list->Elements[i].get());
        }

        // Finish list and return result
        result += "]";
        return result;
    }

    if (ListAtExpr* at = dynamic_cast<ListAtExpr*>(expr))
    {
        return at->ListName + "[" + GenCode(0, at->Index.get()) + "]";     // returns name[index]
    }

    // Length of a list
    if (ListSizeExpr* size = dynamic_cast<ListSizeExpr*>(expr))
    {
        return "len(" + size->ListName + ")";   // returns len(name)
    }
     
    // Unknown type 
    throw std::runtime_error("Unknown expression type");
}


