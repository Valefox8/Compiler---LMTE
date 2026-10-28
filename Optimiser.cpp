#include "Optimiser.h"

Optimiser::Optimiser(){
    this->reachable;
}

void Optimiser::MarkReachable(ScopeExpr* scope)
{
    if (!reachable.insert(scope).second)
    {
        return;
    }

    for (Expr* calledExpr : scope->CalledExprDec)
    {
        if (ScopeExpr* childrenScopeExpr = dynamic_cast<ScopeExpr*>(calledExpr))
            MarkReachable(childrenScopeExpr);
    }
}


void Optimiser::Sweep(ScopeExpr& currentScope)
{
    std::vector<std::unique_ptr<Expr>> retained;

    for (std::unique_ptr<Expr>& expr : currentScope.Body)
    {
        Expr* raw = expr.get();

        if (dynamic_cast<FunctionDecExpr*>(raw) || dynamic_cast<ClassDecExpr*>(raw))
        {
            if (!reachable.count(raw))
                continue;
        }

        ScopeExpr* innerScope = dynamic_cast<ScopeExpr*>(raw);
        if (innerScope)
            Sweep(*innerScope);

        retained.push_back(std::move(expr));
    }

    currentScope.Body = std::move(retained);
}