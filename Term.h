#ifndef TERM_H
#define TERM_H
#include <stddef.h>

enum class TermType {
    Variable,
    Application,
    Abstraction
};

struct Term {
    TermType type;
    union {
        char variable;
        struct {
            Term* leftTerm;
            Term* rightTerm;
        };
        struct {
            char argument;
            Term* trunk;
        };
    };
    
    explicit Term(char variable);
    Term(Term leftTerm, Term rightTerm);
    Term(char argument, Term trunk);
    ~Term();
    Term(const Term& term);
    Term(Term&& term);
    Term& operator=(const Term& term);
    Term& operator=(Term&& term);
    
private:
    void clear();
};

#endif // TERM_H
