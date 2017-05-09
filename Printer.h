#ifndef PRINTER_H
#define PRINTER_H

#include <ostream>
#include <vector>
#include <string>
#include <map>
#include <tuple>
#include "Term.h"

class Printer
{
    std::ostream& mOut;
    std::map<const Term*, std::pair<std::string, std::string>> mSurroundedTerms;
    
public:
    Printer(std::ostream& ostream);
    void printTerm(const Term& term);
    
    void setSurroundedTerms(std::map<const Term*, std::pair<std::string, std::string>> terms);
    
private:
    void printTermPrefixes(const Term& term);
    void printTermSuffixes(const Term& term);
};

#endif // PRINTER_H
