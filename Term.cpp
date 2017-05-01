#include "Term.h"

Term::Term(TermType type) : type(type)
{
    switch(type) {
    case TermType::Variable:
        variable = 0;
        break;
    case TermType::Application:
        leftTerm = nullptr;
        rightTerm = nullptr;
        break;
    case TermType::Abstraction:
        argument = 0;
        trunk = nullptr;
        break;
    }
}
