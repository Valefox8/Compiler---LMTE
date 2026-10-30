#include "Optimiser.h"

Optimiser::Optimiser(){
}



// Record all the declaration of all the scope, see if each are used or not. This starts from the global scope and move along each function scope branch 
void Optimiser::MarkReachableScope(ScopeExpr* scope)
{
    if (!reachable.insert(scope).second)
    {
        return;
    }
    for (Expr* calledExpr : scope->CalledExprDec)
    {
        if (ScopeExpr* childrenScopeExpr = dynamic_cast<ScopeExpr*>(calledExpr))
        {
            // All the if scope are recorded. Function dec expr WITH function calls is also recoreded 
            MarkReachableScope(childrenScopeExpr);   
        } else {

            // Only print, or variable dec if the  itself calls a function are recorded 
            MarkReachableExpr(calledExpr);
        }
    }
}

// See if we can remove VarDecExpr or not
void Optimiser::MarkReachableExpr(Expr* expr)
{
    // If its a variable, preserve it
    if (SayExpr* sayEx = dynamic_cast<SayExpr*>(expr))
    {
        for (Expr* dependency : sayEx->CalledExprDec)
        {
            MarkReachableExpr(dependency);
        }
        return;
    }

    // This will only be called if the print condition above calls it or if the variable itself calls a function
    if (VarDecExpr* varDec = dynamic_cast<VarDecExpr*>(expr))
    {
        
        if (!reachable.insert(varDec).second)
        {
            return;
        }

        for (Expr* dependency : varDec->CalledExprDec)
        {
            MarkReachableExpr(dependency);
        }
        return;
    }
}

// Do a sweep to see what can be removed withou affecting the program
void Optimiser::Sweep(ScopeExpr& currentScope)
{
    std::vector<std::unique_ptr<Expr>> retained;

    for (std::unique_ptr<Expr>& expr : currentScope.Body)
    {
        
        Expr* raw = expr.get();

        // Go through all scopes
        ScopeExpr* innerScope = dynamic_cast<ScopeExpr*>(raw);
        if (innerScope)
        {
            if (!reachable.count(innerScope))
            {
                continue;
            }
                

            Sweep(*innerScope);
        }

        // Delete all unsed variable declaration
        if (VarDecExpr* varDec = dynamic_cast<VarDecExpr*> (raw))
        {
            if (!reachable.count(raw))
            {
                continue;
            }
                 
        }

        // If a variable is used but not printed (eg. Vnum a = 10; Vnum b = a), remove the assignment. 
        if (AssignExpr* assignExpr = dynamic_cast<AssignExpr*>(raw))
        {
            if (IdentifierExpr* idExpr = dynamic_cast<IdentifierExpr*> (assignExpr->Target.get()))
            {
                if (dynamic_cast<Expr*>(currentScope.LookUp(idExpr->Name)->Expression) == nullptr)
                {
                    continue;
                }       
            }
        }

        // These are expression that cant be used since no variable owns them, so they are removed
        if (
            dynamic_cast<NumberExpr*>(raw) ||
            dynamic_cast<BinaryExpr*>(raw) ||
            dynamic_cast<BooleanExpr*>(raw) ||
            dynamic_cast<LetterExpr*>(raw) ||
            dynamic_cast<ListAtExpr*>(raw) ||
            dynamic_cast<ListSizeExpr*>(raw)
        ){
            continue;
        }

        retained.push_back(std::move(expr));
    }

    currentScope.Body = std::move(retained);
}