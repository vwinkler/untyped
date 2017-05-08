#include "Printer.h"

Printer::Printer(std::ostream& ostream)
 : mOut(ostream)
{
}

void Printer::printTerm(const Term& term)
{
    switch(term.type) {
    case TermType::Variable:
        mOut << term.variable;
        break;
    case TermType::Application:
        mOut << "(";
        printTerm(*term.leftTerm);
        mOut << " ";
        printTerm(*term.rightTerm);
        mOut << ")";
        break;
    case TermType::Abstraction:
        mOut << "\\" << term.argument << ".";
        printTerm(*term.trunk);
        break;
    }
}
