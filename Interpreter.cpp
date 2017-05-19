#include "Interpreter.h"
#include <cassert>
#include <memory>
#include <stdexcept>
#include <limits>
#include <sstream>

Interpreter::Interpreter(Term term)
 : mTerm(std::move(term))
{
}

const Term& Interpreter::term() const
{
    return mTerm;
}

const Term* Interpreter::nextRedex() const
{
    return findRedex(&mTerm);
}

bool Interpreter::applyOnce()
{
    Term* redex = findRedex(&mTerm);
    bool result = (redex != nullptr);
    if(result) {
        substitute(redex->leftTerm().argument(), redex->leftTerm().trunk(), redex->rightTerm());
        *redex = redex->leftTerm().trunk();
    }
    return result;
}

void Interpreter::applyRecursively()
{
    while(applyOnce());
}

const Term* Interpreter::findRedex(const Term* term)
{
    assert(term != nullptr);
    
    const Term* result = nullptr;
    switch(term->type()) {
    case TermType::Variable:
        break;
    case TermType::Abstraction:
        result = findRedex(&term->trunk());
        break;
    case TermType::Application:
        if(term->leftTerm().type() == TermType::Abstraction){
            result = term;
        } else {
            result = findRedex(&term->leftTerm());
            if(result == nullptr) {
                result = findRedex(&term->rightTerm());
            }
        }
        break;
    }
    return result;
}

Term* Interpreter::findRedex(Term* term)
{
    return const_cast<Term*>(findRedex(static_cast<const Term*>(term)));
}

void Interpreter::substitute(Variable variable, Term& target, const Term& term)
{
    switch(target.type()) {
    case TermType::Variable:
        if(target.variable() == variable)
            target = term;
        break;
    case TermType::Abstraction:
        if(target.argument() != variable){
            auto newVar = findReplacement(target.argument(), term);
            if(newVar.name() == variable.name())
                newVar = commonVariable(newVar, variable);
            substitute(target.argument(), target.trunk(), Term(newVar));
            target.setArgument(newVar);
            substitute(variable, target.trunk(), term);
        }
        break;
    case TermType::Application:
        substitute(variable, target.leftTerm(), term);
        substitute(variable, target.rightTerm(), term);
        break;
    }
}

Variable Interpreter::findReplacement(Variable variable, const Term& term)
{
    Variable result = variable;
    switch(term.type()) {
    case TermType::Variable:
        if(term.variable() == variable)
            result = term.variable().replacement();
        break;
    case TermType::Abstraction:
        if(term.argument() != variable){
            result = findReplacement(variable, term.trunk());
        }
        break;
    case TermType::Application:
        result = commonVariable(findReplacement(variable, term.leftTerm()),
                          findReplacement(variable, term.rightTerm()));
        break;
    }
    return commonVariable(result, variable);
}
