#include "Term.h"
#include <memory>

Term::Term(char variable)
:   type(TermType::Variable), variable(variable)
{
}

Term::Term(Term leftTerm, Term rightTerm)
:   type(TermType::Application),
    leftTerm(new Term(std::move(leftTerm))),
    rightTerm(new Term(std::move(rightTerm)))
{
}

Term::Term(char argument, Term trunk)
:   type(TermType::Abstraction),
    argument(argument),
    trunk(new Term(std::move(trunk)))
{
}

Term::~Term()
{
    clear();
}

void Term::clear() {
    switch(type) {
    case TermType::Variable:
        break;
    case TermType::Application:
        delete leftTerm;
        leftTerm = nullptr;
        delete rightTerm;
        rightTerm = nullptr;
        break;
    case TermType::Abstraction:
        delete trunk;
        trunk = nullptr;
        break;
    }
}

Term::Term(const Term& term)
{
    type = term.type;
    switch(type) {
    case TermType::Variable:
        variable = term.variable;
        break;
    case TermType::Application:
        leftTerm = new Term(*term.leftTerm);
        rightTerm = new Term(*term.rightTerm);
        break;
    case TermType::Abstraction:
        argument = term.argument;
        trunk = new Term(*term.trunk);
        break;
    }
}

Term::Term(Term&& term)
{
    type = term.type;
    switch(type) {
    case TermType::Variable:
        variable = term.variable;
        break;
    case TermType::Application:
        leftTerm = term.leftTerm;
        term.leftTerm = nullptr;
        rightTerm = term.rightTerm;
        term.rightTerm = nullptr;
        break;
    case TermType::Abstraction:
        argument = term.argument;
        trunk = term.trunk;
        term.trunk = nullptr;
        break;
    }
}

Term& Term::operator=(const Term& term)
{
    clear();
    type = term.type;
    switch(type) {
    case TermType::Variable:
        variable = term.variable;
        break;
    case TermType::Application:
        leftTerm = new Term(*term.leftTerm);
        rightTerm = new Term(*term.rightTerm);
        break;
    case TermType::Abstraction:
        argument = term.argument;
        trunk = new Term(*term.trunk);
        break;
    }
    return *this;
}


Term& Term::operator=(Term&& term)
{
    if(type != term.type){
        clear();
        type = term.type;
        switch(type) {
        case TermType::Variable:
            variable = term.variable;
            break;
        case TermType::Application:
            leftTerm = term.leftTerm;
            term.leftTerm = nullptr;
            rightTerm = term.rightTerm;
            term.rightTerm = nullptr;
            break;
        case TermType::Abstraction:
            argument = term.argument;
            trunk = term.trunk;
            term.trunk = nullptr;
            break;
        }
    } else {
        switch(term.type) {
        case TermType::Variable:
            std::swap(variable, term.variable);
            break;
        case TermType::Application:
            std::swap(leftTerm, term.leftTerm);
            std::swap(rightTerm, term.rightTerm);
            break;
        case TermType::Abstraction:
            std::swap(argument, term.argument);
            std::swap(trunk, term.trunk);
            break;
        }
    }
    return *this;
}
