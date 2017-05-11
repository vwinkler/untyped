#include "Term.h"
#include <cassert>
#include <memory>
#include <vector>

Term::Term(char variable)
    :   mType(TermType::Variable), mVariable(variable)
{
}

Term::Term(Term leftTerm, Term rightTerm)
    :   mType(TermType::Application),
        mLeftTerm(new Term(std::move(leftTerm))),
        mRightTerm(new Term(std::move(rightTerm)))
{
}

Term::Term(char argument, Term trunk)
    :   mType(TermType::Abstraction),
        mArgument(argument),
        mTrunk(new Term(std::move(trunk)))
{
}

Term::~Term()
{
    clear();
}

void Term::clear()
{
    switch(mType) {
    case TermType::Variable:
        break;
    case TermType::Application:
        delete mLeftTerm;
        mLeftTerm = nullptr;
        delete mRightTerm;
        mRightTerm = nullptr;
        break;
    case TermType::Abstraction:
        delete mTrunk;
        mTrunk = nullptr;
        break;
    }
}


Term::Term(const Term& term)
{
    mType = term.mType;
    switch(mType) {
    case TermType::Variable:
        mVariable = term.mVariable;
        break;
    case TermType::Application:
        mLeftTerm = new Term(*term.mLeftTerm);
        mRightTerm = new Term(*term.mRightTerm);
        break;
    case TermType::Abstraction:
        mArgument = term.mArgument;
        mTrunk = new Term(*term.mTrunk);
        break;
    }
}

Term::Term(Term&& term)
{
    mType = term.mType;
    switch(mType) {
    case TermType::Variable:
        mVariable = term.mVariable;
        break;
    case TermType::Application:
        mLeftTerm = term.mLeftTerm;
        term.mLeftTerm = nullptr;
        mRightTerm = term.mRightTerm;
        term.mRightTerm = nullptr;
        break;
    case TermType::Abstraction:
        mArgument = term.mArgument;
        mTrunk = term.mTrunk;
        term.mTrunk = nullptr;
        break;
    }
}

Term& Term::operator=(const Term& term)
{
    if(this != &term) {
        clear();
        mType = term.mType;
        switch(mType) {
        case TermType::Variable:
            mVariable = term.mVariable;
            break;
        case TermType::Application:
            mLeftTerm = new Term(*term.mLeftTerm);
            mRightTerm = new Term(*term.mRightTerm);
            break;
        case TermType::Abstraction:
            mArgument = term.mArgument;
            mTrunk = new Term(*term.mTrunk);
            break;
        }
    }
    return *this;
}


Term& Term::operator=(Term&& term)
{
    assert(!isSubterm(term, *this));
    if(mType != term.mType) {
        std::vector<Term*> toDelete;
        switch(mType) {
        case TermType::Variable:
            break;
        case TermType::Application:
            toDelete.push_back(mLeftTerm);
            toDelete.push_back(mRightTerm);
            break;
        case TermType::Abstraction:
            toDelete.push_back(mTrunk);
            break;
        }
        
        mType = term.mType;
        switch(mType) {
        case TermType::Variable:
            mVariable = term.mVariable;
            break;
        case TermType::Application:
            mLeftTerm = term.mLeftTerm;
            term.mLeftTerm = nullptr;
            mRightTerm = term.mRightTerm;
            term.mRightTerm = nullptr;
            break;
        case TermType::Abstraction:
            mArgument = term.mArgument;
            mTrunk = term.mTrunk;
            term.mTrunk = nullptr;
            break;
        }
        for(auto x : toDelete)
            delete x;
    } else {
        switch(term.mType) {
        case TermType::Variable:
            std::swap(mVariable, term.mVariable);
            break;
        case TermType::Application:
            std::swap(mLeftTerm, term.mLeftTerm);
            std::swap(mRightTerm, term.mRightTerm);
            break;
        case TermType::Abstraction:
            std::swap(mArgument, term.mArgument);
            std::swap(mTrunk, term.mTrunk);
            break;
        }
    }
    return *this;
}

TermType Term::type() const {
    return mType;
}

char Term::variable() const {
    assert(mType == TermType::Variable);
    return mVariable;
}

void Term::setVariable(char variable)
{
    assert(mType == TermType::Variable);
    mVariable = variable;
}

const Term& Term::leftTerm() const
{
    assert(mType == TermType::Application);
    return *mLeftTerm;
}

Term& Term::leftTerm()
{
    assert(mType == TermType::Application);
    return *mLeftTerm;
}

void Term::setLeftTerm(Term term) {
    assert(mType == TermType::Application);
    delete mLeftTerm;
    mLeftTerm = new Term(std::move(term));
}

const Term& Term::rightTerm() const
{
    assert(mType == TermType::Application);
    return *mRightTerm;
}

Term& Term::rightTerm()
{
    assert(mType == TermType::Application);
    return *mRightTerm;
}

void Term::setRightTerm(Term term) {
    assert(mType == TermType::Application);
    delete mRightTerm;
    mRightTerm = new Term(std::move(term));
}

char Term::argument() const
{
    assert(mType == TermType::Abstraction);
    return mArgument;
}

void Term::setArgument(char argument)
{
    assert(mType == TermType::Abstraction);
    mArgument = argument;
}

const Term& Term::trunk() const
{
    assert(mType == TermType::Abstraction);
    return *mTrunk;
}

Term& Term::trunk()
{
    assert(mType == TermType::Abstraction);
    return *mTrunk;
}

void Term::setTrunk(Term trunk)
{
    assert(mType == TermType::Abstraction);
    mTrunk = new Term(std::move(trunk));
}

bool Term::isSubterm(const Term& term, const Term& sub)
{
    bool result;
    switch(term.mType) {
    case TermType::Variable:
        result = false;
        break;
    case TermType::Application:
        result = &term.leftTerm() == &sub
            || &term.rightTerm() == &sub
            || isSubterm(term.leftTerm(), sub)
            || isSubterm(term.rightTerm(), sub);
        break;
    case TermType::Abstraction:
        result = &term.trunk() == &sub
            || isSubterm(term.trunk(), sub);
        break;
    }
    return result;
}

