#include <iostream>
#include <sstream>
#include "Term.h"
#include "Parser.h"
#include "Printer.h"
#include "Interpreter.h"

int main(int argc, char **argv) {
    std::cout << "insert lambda-expression: ";
    std::string input;
    std::getline(std::cin, input);
    std::istringstream stream(input);
    Parser parser(stream);
    Term term('\0');
    try {
        term = parser.buildSyntaxTree();
    } catch(const SyntaxError& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    std::cout << "execution ...";
    
    Printer printer(std::cout);
    Interpreter interpreter(term);
    bool changed;
    do {
        const Term* redex = interpreter.nextRedex();
        printer.setSurroundedTerms({std::make_pair(redex, std::make_pair("\033[1;31m", "\033[0m"))});
        std::cout << std::endl;
        printer.printTerm(interpreter.term());
        changed = interpreter.applyOnce();
    } while(changed);
    std::cout << std::endl;
    return 0;
}
