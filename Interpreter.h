#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Term.h"

class Interpreter
{
    Term mTerm;
    
public:
    Interpreter(Term term);
    
    const Term& term() const;
    const Term* nextRedex() const;
    
    bool applyOnce();
    void applyRecursively();
    
private:
    static const Term* findRedex(const Term* term);
    static Term* findRedex(Term* term);
    static void substitute(Variable variable, Term& target, const Term& term);
    static Variable findReplacement(Variable variable, const Term& term);
};

#endif // INTERPRETER_H
