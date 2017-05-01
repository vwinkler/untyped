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
    Term* parent;
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
    
    Term(TermType type);
};

#endif // TERM_H
