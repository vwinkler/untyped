#ifndef TERM_H
#define TERM_H
#include <stddef.h>
#include <functional>
#include <vector>

enum class TermType {
    Variable,
    Application,
    Abstraction
};

class Variable {
public:
    Variable() = default;
    Variable(char var);
    Variable replacement();
    char name() const;
    unsigned char id() const;
    
private:
    char mName;
    unsigned char mId;
};

bool operator==(Variable left, Variable right);
bool operator!=(Variable left, Variable right);
Variable commonVariable(Variable left, Variable right);

class Term {  
public:
    explicit Term(Variable variable);
    Term(Term leftTerm, Term rightTerm);
    Term(Variable argument, Term trunk);
    ~Term();
    Term(const Term& term);
    Term(Term&& term);
    Term& operator=(const Term& term);
    Term& operator=(Term&& term);
    
    TermType type() const;
    Variable variable() const;
    void setVariable(Variable variable);
    const Term& leftTerm() const;
    Term& leftTerm();
    void setLeftTerm(Term term);
    const Term& rightTerm() const;
    Term& rightTerm();
    void setRightTerm(Term term);
    Variable argument() const;
    void setArgument(Variable argument);
    const Term& trunk() const;
    Term& trunk();
    void setTrunk(Term trunk);
    
private:
    TermType mType;
    union {
        Variable mVariable;
        struct {
            Term* mLeftTerm;
            Term* mRightTerm;
        } mApplication;
        struct {
            Variable mArgument;
            Term* mTrunk;
        } mAbstraction;
    };
    
    void clear();
    std::vector<Term*> salvage();
    bool isSubterm(const Term& term, const Term& sub);
};

#endif // TERM_H
