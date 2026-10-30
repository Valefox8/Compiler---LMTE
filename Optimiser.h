// If you want to sope the optimiser from running, remove its declaration and usage in the main.cpp file

#ifndef OPTIMISER_H
#define OPTIMISER_H

#include <vector>
#include <memory>
#include <string>
#include <unordered_set>


#include "Token.h"
#include "Expr.h"
#include "Parser.h"


class Optimiser{
    private:
    std::unordered_set<Expr*> reachable;

    public:
    Optimiser();
    void MarkReachableScope(ScopeExpr* scope);
    void MarkReachableExpr(Expr* expr);

    // IMPORTANT: USING THIS WILL ACTUALLY REMOVE UNUSED EXPRESSION IN GLOBAL SCOPE, MAKING SYMBOL TABLE USELESS. 
    // LET ME KNOW IF U WANNA KEEP UNUSED EXPRESSION, I'LL THINK OF SOMETHING ELSE INSTEAD.
    void Sweep(ScopeExpr& currentScope);


};

#endif