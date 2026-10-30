#include"Expr.h"

// See whether a variable was ever declared whether in this scope or previous parent scopes
bool ScopeExpr::Contain (const std::string& varName)
{
    if (Table.count(varName) == 1)
    {
        return true;
    }
    if (ParentScope != nullptr)
    {
        return ParentScope->Contain(varName);
    }
    return false;      
}

// See whether a variable was declared in currect scope
bool ScopeExpr::ContainLocal (const std::string& varName)
{
    if (Table.count(varName) == 1)
    {
        return true;
    }
    return false;
}

// Return an entry to the symbol table from current to global scope
// Is also a way to check if a symbol was added or not 
SymbolEntry* ScopeExpr::LookUp(const std::string& varName)
{
    if (Table.count(varName) == 1)
    {
        return Table[varName].get();
    }

    if (ParentScope != nullptr)
    {
        return ParentScope->LookUp(varName);
    }

    throw std::runtime_error("Cant find variable: " + varName);      
}

// Return an entry to the symbol table from current scope
// Is also a way to check if a symbol was added or not 
SymbolEntry* ClassDecExpr::LookUpLocal(const std::string& varName)
{
    if (Table.count(varName) == 1)
    {
        return Table[varName].get();
    }

    if (ClassicClass != nullptr)
    {
        SymbolEntry* returnSymbol = ClassicClass->LookUpLocal(varName);
        if (returnSymbol->Access == TokenType::Private)
        {
            throw std::runtime_error("Attribute: " + varName + " is a private attribute and cant be accessed by children classes");
        }
        return returnSymbol;
    }

    throw std::runtime_error("Cant find in local scope: " + this->Name + " value: "+ varName);      
}

// Insert into current symbol table, will ignore if variable is declared in higher up scopes
void ScopeExpr::Insert(const std::string& varName, std::unique_ptr<SymbolEntry> entry)
{
    if (Table.count(varName) == 1)
    {
        throw std::runtime_error("Variable already declared: " + varName);
    }

    // Will not allow nullptr
    if (entry == nullptr)
    {
        throw std::runtime_error("This entry expression is null: " + varName);
    }

    Table[varName] = std::move(entry);
}


bool CheckUsage::MarkUsed(Expr* expr)
{
    if (ClassDecExpr* classExpr = dynamic_cast<ClassDecExpr*>(expr)){
        CalledExprDec.insert(classExpr->LookUp("SELF")->Expression);
    }
    return CalledExprDec.insert(expr).second;
}

// Continuously search through parent scope to find a class declaration expression. 
// Needed for variable dec and method call in the scope of a class dec
// Return type: null if cant find a class dec
// Pointer to Class dec expr if can find one
ClassDecExpr* ScopeExpr::GetParentClass(){

    if (ParentScope == nullptr){

        return nullptr;
    }

    ClassDecExpr* parentClassDecExpr;

    if (parentClassDecExpr = dynamic_cast<ClassDecExpr*>(ParentScope)) {
        return parentClassDecExpr;
    }

    return ParentScope->GetParentClass();
}


// Just added to check the scope, will probably delete or imporve for better testing
std::string ScopeExpr::GetNames(){
    std::string names;
    for (auto& pair : Table)
    {
        names += pair.first + "\n";
    }
    return names;
}


// Returning a vector of poiters to each parameter declaration expression stored inside a function expression
std::vector<ParameterExpr*> ScopeExpr::GetFunctionPara(std::string funcName)
{
    
    std::vector<ParameterExpr*> allPara;
    
    // Check if the scope contains the function
    SymbolEntry* symbol;
    if (dynamic_cast<ClassDecExpr*>(this) && dynamic_cast<ClassDecExpr*>(this)->ClassicClass && !Contain(funcName))
    {
        symbol = dynamic_cast<ClassDecExpr*>(this)->LookUpLocal(funcName);
    } else {
        symbol = LookUp(funcName);
    }

    if (symbol->SymK != SymbolKind::Method && symbol->SymK != SymbolKind::Function) {
        throw std::runtime_error("Not a function and cant return parameter: " + funcName);
    }

    // Iterating through each Prameter Expression stored inside a function expression
    FunctionDecExpr* funcExpr = dynamic_cast<FunctionDecExpr*>(symbol->Expression);

    if (!funcExpr) {
        throw std::runtime_error("Not a function expression and cant return parameter: " + funcName);
    }

    size_t length = funcExpr->Param.size();

    for (size_t i = 0; i < length; i++)
    {
        allPara.push_back(funcExpr->Param[i].get());
    }

    return allPara;
};


void ScopeExpr::PrintSymbolTable() const
{
    std::cout << "\n===== "+this->Name+" =====\n";

    for (const auto& [name, entry] : Table)
    {
        std::cout << "Name: " << name << '\n';

        if (entry == nullptr)
        {
            std::cout << "  SymbolEntry is nullptr\n";
            continue;
        }

        std::cout << "  TokenType: "
                  << static_cast<int>(entry->Type) << '\n';

        std::cout << "  SymbolKind: "
                  << static_cast<int>(entry->SymK) << '\n';

        std::cout << "  Expression address: "
                  << static_cast<const void*>(entry->Expression)
                  << '\n';

        if (entry->Expression == nullptr)
        {
            std::cout << "  Expression is nullptr\n";
        }
        else
        {
            std::cout << "  Expression type: "
                      << typeid(*entry->Expression).name()
                      << '\n';

        }

        std::cout << "------------------------\n";
    }

    std::cout << "========================\n";
}