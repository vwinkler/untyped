#ifndef PRINTER_H
#define PRINTER_H

#include <ostream>
#include <vector>
#include <string>
#include "Term.h"

class Printer
{
    std::ostream& mOut;
    
public:
    Printer(std::ostream& ostream);
    void printTerm(const Term* term);
};

#endif // PRINTER_H
