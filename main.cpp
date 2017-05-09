#include <iostream>
#include <sstream>
#include "Term.h"
#include "Parser.h"
#include "Printer.h"
#include "Interpreter.h"

int main(int argc, char **argv) {
    std::string input = "((\\x.x \\t.t) v)";
    std::istringstream stream(input);
    Parser parser(stream);
    Term term('\0');
    try {
        term = parser.buildSyntaxTree();
    } catch(const SyntaxError& ex) {
        std::cerr << ex.what() << std::endl;
    }
    
    Printer printer(std::cout);
    printer.printTerm(term);
    std::cout << std::endl;
    
    Interpreter interpreter(term);
    const Term* redex = interpreter.nextRedex();
    printer.setSurroundedTerms({std::make_pair(redex, std::make_pair("{{", "}}"))});
    printer.printTerm(interpreter.term());
    std::cout << std::endl;
    return 0;
}
