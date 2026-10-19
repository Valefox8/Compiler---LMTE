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

std::string CodeGen::Indent(std::string text){
    std::string result = "    ";   // Indent the first line

    for (size_t i = 0; i < text.length(); i++)
    {
        result += text[i];

        if (text[i] == '\n')
        {
            result += "    ";
        }
    }

    return result;
}


std::string CodeGen::GenCode(Expr* expr){

    // Return the number as its text value

    if (NumberExpr* number = dynamic_cast<NumberExpr*>(expr))
    {
        return number->Value;
    }

    // Return the variable name
    if (IdentifierExpr* identifier = dynamic_cast<IdentifierExpr*>(expr))
    {
        std::string result;
        
        // See if its 'selfish ->' or an 'object->' calling  
        result += identifier->Name;
        return result;
    }

    // Expression, generate both sides and the operator before printing
    if (BinaryExpr* binary = dynamic_cast<BinaryExpr*>(expr))
    {
        std::string left = GenCode(binary->Left.get());
        std::string right = GenCode(binary->Right.get());
        std::string op = Operator(binary->Operator);

        //adds tabs for python standard
        return "(" + left + " " + op + " " + right + ")";
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

    // Variable/Attribute declaration
    if (VarDecExpr* decl = dynamic_cast<VarDecExpr*>(expr))
    {
        std::string result;

        result += GenCode(decl->Name.get());

        switch (decl->Access)
        {
            case TokenType::Protected:
                result += "_";
                break;
            case TokenType::Private:
                result += "__";
                break;
        }
        result += "=";
        result += GenCode(decl->Value.get());

        return result;  // Returns and constructs the assignement with the name, equals sign and value we just got
    }

    // if (VarDecExpr* decl = dynamic_cast<VarDecExpr*>(expr))
    // {
    //     return "success";
    // }


    if(dynamic_cast<HandbrakeExpr*>(expr)){
        return "break";
    }
    
    if(IterationExpr* iteration = dynamic_cast<IterationExpr*>(expr))
    {
        std::string result =  "while(True):";

        for (size_t i = 0; i < iteration->Body.size(); i++)
            result += "\n" + Indent(GenCode(iteration->Body[i].get()));
                            
        return result;
    }

    if(ConditionalStatementStructExpr* conditionalStatementStruct = dynamic_cast<ConditionalStatementStructExpr*>(expr))
    {    
        std::string result;

        for(const auto& structIfElseStatements : conditionalStatementStruct->ConditionalStatements){
            auto* conditionStatement = structIfElseStatements.get();
            TokenType type = conditionStatement->Type;
            std::string conditions;

            if (type == TokenType::IguessIf)
            {
                conditions = GenCode(conditionStatement->Condition.get());
                result += "if " + conditions + ":";
            } else if (type == TokenType::Guessthis) 
            { 
                conditions = GenCode(conditionStatement->Condition.get());
                result += "\nelif " + conditions + ":";
            } else {
                result += "\nelse:";
            }
            
            for (size_t i = 0; i < conditionStatement->Body.size(); i++)
            {
                result += "\n" + Indent(GenCode(conditionStatement->Body[i].get()));                
            }                

        }
        return result;
    }


    // Creating the list
    if(ListExpr* list = dynamic_cast<ListExpr*>(expr)){

        std::string result;

        // Grabs the name and the strat of the python list syntax
        if (list->Name == "anotherone")
        {
            result += "[";
        } else {
            result += list->Name + " = [";
        }

        // For each element in the elements vector, create a new python list entry 
        for(size_t i = 0; i < list->Elements.size(); i++){
            if(i > 0){
                result += ", ";
            }

            result += GenCode(list->Elements[i].get());
        }

        // Finish list and return result
        result += "]";
        return result;
    }

    if (ListAtExpr* at = dynamic_cast<ListAtExpr*>(expr))
    {
        return at->ListName + "[" + GenCode(at->Index.get()) + "]";     // returns name[index]
    }

    // Length of a list
    if (ListSizeExpr* size = dynamic_cast<ListSizeExpr*>(expr))
    {
        return "len(" + size->ListName + ")";   // returns len(name)
    }
    
    // Funciton/Method Declaration
    if (FunctionDecExpr* function = dynamic_cast<FunctionDecExpr*>(expr))
    {

        std::string result = "def ";

        switch (function->Access)
        {
            case TokenType::Protected:
                result += "_";
                break;
            case TokenType::Private:
                result += "__";
                break;
        }
        // Check if its a constructor
        result += (function->Name == "SELF") ? "__init__ (" :  function->Name + " (";

        // Adding parameters
        if (function->IsMethod) 
        {
            result += "selfish, ";
        }
        auto& para = function->Param;
        for (size_t i = 0; i < para.size(); ++i) 
        {
            result += para[i]->Name;
            if (i < para.size() - 1) 
            { 
                result += ", ";
            }
        }        
        result += "):";

        // Each statement goes on its own indented line
        for (size_t i = 0; i < function->Body.size(); i++)
        {
            result += "\n" + Indent(GenCode(function->Body[i].get()));
        }
        
        if (function->Body.size() == 0)
        {
            result += "\n    pass";
        }

        return result;
    }

    
    // Return statement
    if (ReturnExpr* leave = dynamic_cast<ReturnExpr*>(expr))
    {
        return "return " + GenCode(leave->Value.get());
    }
    // Function call
    if (FunctionCallExpr* call = dynamic_cast<FunctionCallExpr*>(expr))
    {
        std::string result;

        
        // Generating parameters 
        result += call->FuncDecExpr->Name + "(";
        for (size_t i = 0; i < call->Args.size(); i++)
        {
            if (i > 0)
            {
                result += ", ";
            }

            result += GenCode(call->Args[i].get());
        }

        result += ")";
        return result;
    }


    // Class declaration
    if (ClassDecExpr* classDec = dynamic_cast<ClassDecExpr*>(expr))
    {
        std::string result = "class " + classDec->Name + ":";

        // Generating each method (plz keep in mind the body only stores methods)
        for (size_t i = 0; i < classDec->Body.size(); i++)
        {
            result += "\n" + Indent(GenCode(classDec->Body[i].get()));
        }

        // Empty class
        if (classDec->Body.size() == 0)
        {
            result += "\n    pass";
        }

        return result;
    }


    // Reassignment
    if (AssignExpr* assign = dynamic_cast<AssignExpr*>(expr))
    {
        std::string result;
        result += GenCode(assign->Target.get());

        result += "=";

        result += GenCode(assign->Value.get());
        return result;
    }

    // Generating objects
    if (ObjectDecExpr* object = dynamic_cast<ObjectDecExpr*>(expr))
    {
        std::string result = object->Name + " = " + object->ClassDefExpr->Name + "(";

        // Generating arguments
        for (size_t i = 0; i < object->Args.size(); i++)
        {
            if (i > 0)
            {
                result += ", ";
            }
            result += GenCode(object->Args[i].get());
        }

        result += ")";
        return result;
    }


    if (AttReferenceExpr* attRef = dynamic_cast<AttReferenceExpr*>(expr))
    {
        std::string result;

        result += GenCode(attRef->Primary.get());

        result += ".";

        result += GenCode(attRef->Attribute.get());

        return result;  // Returns and constructs the assignement with the name, equals sign and value we just got
    }


    // Unknown type 
    throw std::runtime_error("Unknown expression type");
}


