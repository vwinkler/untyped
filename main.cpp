#include <iostream>
#include <sstream>
#include "Term.h"
#include "Parser.h"
#include "Printer.h"

int main(int argc, char **argv) {
    std::string input = "((\\x.x \\y.y) v)";
    std::istringstream stream(input);
    Parser parser(stream);
    Term* term;
    try {
        term = parser.buildSyntaxTree();
    } catch(const SyntaxError& ex) {
        std::cerr << ex.what() << std::endl;
    }
    
    Printer printer(std::cout);
    printer.printTerm(term);
    return 0;
}
