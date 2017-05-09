#ifndef TERM_H
#define TERM_H
#include <stddef.h>

enum class TermType {
    Variable,
    Application,
    Abstraction
};

struct Term {
private:
    TermType mType;
    union {
        char mVariable;
        struct {
            Term* mLeftTerm;
            Term* mRightTerm;
        };
        struct {
            char mArgument;
            Term* mTrunk;
        };
    };
    
public:
    explicit Term(char variable);
    Term(Term leftTerm, Term rightTerm);
    Term(char argument, Term trunk);
    ~Term();
    Term(const Term& term);
    Term(Term&& term);
    Term& operator=(const Term& term);
    Term& operator=(Term&& term);
    
    TermType type() const;
    char variable() const;
    void setVariable(char variable);
    const Term& leftTerm() const;
    Term& leftTerm();
    void setLeftTerm(Term term);
    const Term& rightTerm() const;
    Term& rightTerm();
    void setRightTerm(Term term);
    char argument() const;
    void setArgument(char argument);
    const Term& trunk() const;
    Term& trunk();
    void setTrunk(Term trunk);
    
private:
    void clear();
};

#endif // TERM_H
