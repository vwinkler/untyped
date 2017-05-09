#include "Printer.h"

Printer::Printer(std::ostream& ostream)
 : mOut(ostream)
{
}

void Printer::printTerm(const Term& term)
{
    printTermPrefixes(term);
    
    switch(term.type()) {
    case TermType::Variable:
        mOut << term.variable();
        break;
    case TermType::Application:
        mOut << "(";
        printTerm(term.leftTerm());
        mOut << " ";
        printTerm(term.rightTerm());
        mOut << ")";
        break;
    case TermType::Abstraction:
        mOut << "\\" << term.argument() << ".";
        printTerm(term.trunk());
        break;
    }
    
    printTermSuffixes(term);
}

void Printer::setSurroundedTerms(std::map<const Term *, std::pair<std::string, std::string>> terms)
{
    mSurroundedTerms = std::move(terms);
}

void Printer::printTermPrefixes(const Term& term)
{
    auto it = mSurroundedTerms.find(&term);
    if(it != mSurroundedTerms.end()) {
        mOut << it->second.first;
    }
}

void Printer::printTermSuffixes(const Term& term)
{
    auto it = mSurroundedTerms.find(&term);
    if(it != mSurroundedTerms.end()) {
        mOut << it->second.second;
    }
}
