#include "Term.h"
#include <cassert>
#include <memory>
#include <limits>
#include <sstream>

Variable::Variable(char var)
    :   mName(var), mId(0)
{
}

Variable Variable::replacement()
{
    Variable result(*this);
    if(result.mId == std::numeric_limits<unsigned char>::max()){
        std::stringstream s;
        s << "can not replace variable, limit ("
        << std::numeric_limits<unsigned char>::max()
        << ") of replacements for '"
        << result.mName << "' exceeded";
        throw std::runtime_error(s.str());
    }
    
    ++result.mId;
    return result;
}

char Variable::name() const
{
    return mName;
}

unsigned char Variable::id() const
{
    return mId;
}

bool operator==(Variable left, Variable right)
{
    return (left.id() == right.id()) && (left.name() == right.name());
}

bool operator!=(Variable left, Variable right)
{
    return !(left == right);
}

Variable commonVariable(Variable left, Variable right)
{
    assert(left.name() == right.name());
    return left.id() < right.id() ? right : left;
}

Term::Term(Variable variable)
    :   mType(TermType::Variable), mVariable(variable)
{
}

Term::Term(Term leftTerm, Term rightTerm)
    :   mType(TermType::Application),
        mApplication{new Term(std::move(leftTerm)), new Term(std::move(rightTerm))}
{
}

Term::Term(Variable argument, Term trunk)
    :   mType(TermType::Abstraction),
        mAbstraction{argument, new Term(std::move(trunk))}
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
        delete mApplication.mLeftTerm;
        mApplication.mLeftTerm = nullptr;
        delete mApplication.mRightTerm;
        mApplication.mRightTerm = nullptr;
        break;
    case TermType::Abstraction:
        delete mAbstraction.mTrunk;
        mAbstraction.mTrunk = nullptr;
        break;
    }
}

std::vector<Term*> Term::salvage()
{
    std::vector<Term*> toDelete;
    switch(mType) {
    case TermType::Variable:
        break;
    case TermType::Application:
        toDelete.push_back(mApplication.mLeftTerm);
        toDelete.push_back(mApplication.mRightTerm);
        break;
    case TermType::Abstraction:
        toDelete.push_back(mAbstraction.mTrunk);
        break;
    }
    return toDelete;
}


Term::Term(const Term& term)
{
    mType = term.mType;
    switch(mType) {
    case TermType::Variable:
        mVariable = term.mVariable;
        break;
    case TermType::Application:
        mApplication.mLeftTerm = new Term(*term.mApplication.mLeftTerm);
        mApplication.mRightTerm = new Term(*term.mApplication.mRightTerm);
        break;
    case TermType::Abstraction:
        mAbstraction.mArgument = term.mAbstraction.mArgument;
        mAbstraction.mTrunk = new Term(*term.mAbstraction.mTrunk);
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
        mApplication.mLeftTerm = term.mApplication.mLeftTerm;
        term.mApplication.mLeftTerm = nullptr;
        mApplication.mRightTerm = term.mApplication.mRightTerm;
        term.mApplication.mRightTerm = nullptr;
        break;
    case TermType::Abstraction:
        mAbstraction.mArgument = term.mAbstraction.mArgument;
        mAbstraction.mTrunk = term.mAbstraction.mTrunk;
        term.mAbstraction.mTrunk = nullptr;
        break;
    }
}

Term& Term::operator=(const Term& term)
{
    if(this != &term) {
        auto toDelete = salvage();
        mType = term.mType;
        switch(mType) {
        case TermType::Variable:
            mVariable = term.mVariable;
            break;
        case TermType::Application:
            mApplication.mLeftTerm = new Term(*term.mApplication.mLeftTerm);
            mApplication.mRightTerm = new Term(*term.mApplication.mRightTerm);
            break;
        case TermType::Abstraction:
            mAbstraction.mArgument = term.mAbstraction.mArgument;
            mAbstraction.mTrunk = new Term(*term.mAbstraction.mTrunk);
            break;
        }
        for(auto x : toDelete)
            delete x;
    }
    return *this;
}


Term& Term::operator=(Term&& term)
{
    assert(!isSubterm(term, *this));
    if(mType != term.mType) {
        auto toDelete = salvage();
        
        mType = term.mType;
        switch(mType) {
        case TermType::Variable:
            mVariable = term.mVariable;
            break;
        case TermType::Application:
            mApplication.mLeftTerm = term.mApplication.mLeftTerm;
            term.mApplication.mLeftTerm = nullptr;
            mApplication.mRightTerm = term.mApplication.mRightTerm;
            term.mApplication.mRightTerm = nullptr;
            break;
        case TermType::Abstraction:
            mAbstraction.mArgument = term.mAbstraction.mArgument;
            mAbstraction.mTrunk = term.mAbstraction.mTrunk;
            term.mAbstraction.mTrunk = nullptr;
            break;
        }
        for(auto x : toDelete)
            delete x;
    } else {
        switch(mType) {
        case TermType::Variable:
            std::swap(mVariable, term.mVariable);
            break;
        case TermType::Application:
            std::swap(mApplication.mLeftTerm, term.mApplication.mLeftTerm);
            std::swap(mApplication.mRightTerm, term.mApplication.mRightTerm);
            break;
        case TermType::Abstraction:
            std::swap(mAbstraction.mArgument, term.mAbstraction.mArgument);
            std::swap(mAbstraction.mTrunk, term.mAbstraction.mTrunk);
            break;
        }
    }
    return *this;
}

TermType Term::type() const {
    return mType;
}

Variable Term::variable() const {
    assert(mType == TermType::Variable);
    return mVariable;
}

void Term::setVariable(Variable variable)
{
    assert(mType == TermType::Variable);
    mVariable = variable;
}

const Term& Term::leftTerm() const
{
    assert(mType == TermType::Application);
    return *mApplication.mLeftTerm;
}

Term& Term::leftTerm()
{
    assert(mType == TermType::Application);
    return *mApplication.mLeftTerm;
}

void Term::setLeftTerm(Term term) {
    assert(mType == TermType::Application);
    delete mApplication.mLeftTerm;
    mApplication.mLeftTerm = new Term(std::move(term));
}

const Term& Term::rightTerm() const
{
    assert(mType == TermType::Application);
    return *mApplication.mRightTerm;
}

Term& Term::rightTerm()
{
    assert(mType == TermType::Application);
    return *mApplication.mRightTerm;
}

void Term::setRightTerm(Term term) {
    assert(mType == TermType::Application);
    delete mApplication.mRightTerm;
    mApplication.mRightTerm = new Term(std::move(term));
}

Variable Term::argument() const
{
    assert(mType == TermType::Abstraction);
    return mAbstraction.mArgument;
}

void Term::setArgument(Variable argument)
{
    assert(mType == TermType::Abstraction);
    mAbstraction.mArgument = argument;
}

const Term& Term::trunk() const
{
    assert(mType == TermType::Abstraction);
    return *mAbstraction.mTrunk;
}

Term& Term::trunk()
{
    assert(mType == TermType::Abstraction);
    return *mAbstraction.mTrunk;
}

void Term::setTrunk(Term trunk)
{
    assert(mType == TermType::Abstraction);
    mAbstraction.mTrunk = new Term(std::move(trunk));
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

