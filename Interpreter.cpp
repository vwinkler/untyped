#include "Interpreter.h"
#include <cassert>
#include <memory>

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
    }
    return result;
}

Term* Interpreter::findRedex(Term* term)
{
    return const_cast<Term*>(findRedex(static_cast<const Term*>(term)));
}
