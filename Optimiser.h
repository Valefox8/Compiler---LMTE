#ifndef OPTIMISER_H
#define OPTIMISER_H

#include <vector>
#include <memory>
#include <stdexcept>
#include <map>
#include <string>
#include <iostream>
#include <unordered_set>


#include "Token.h"
#include "Expr.h"
#include "Parser.h"


class Optimiser{
    private:
    std::unordered_set<Expr*> reachable;

    public:
    Optimiser();
    void MarkReachable(ScopeExpr* scope);
    void Sweep(ScopeExpr& currentScope);

};

#endif