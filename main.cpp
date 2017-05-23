#include <iostream>
#include <sstream>
#include "Term.h"
#include "Parser.h"
#include "Printer.h"
#include "Interpreter.h"

void processExpression(const std::string& expr) {
    std::istringstream stream(expr);
    Parser parser(stream);
    Term term('\0');
    try {
        term = parser.buildSyntaxTree();
        //std::cout << "execution ...\n";
        
        Printer printer(std::cout);
        Interpreter interpreter(term);
        bool changed;
        do {
            const Term* redex = interpreter.nextRedex();
            printer.setSurroundedTerms({std::make_pair(redex, std::make_pair("\033[1;31m", "\033[0m"))});
            printer.printTerm(interpreter.term());
            changed = interpreter.applyOnce();
            std::cout << "\n";
        } while(changed);
        //std::cout << std::endl;
    } catch(const SyntaxError& ex) {
        std::cerr << ex.what() << std::endl;
    }
}

void printHelp() {
    std::cout
    << ":?\t this help\n"
    << ":q\t quit\n";
}

int main(int argc, char **argv) {
    bool keepLooping = true;
    while(keepLooping){
        //std::cout << "insert lambda-expression: ";
        std::string input;
        std::getline(std::cin, input);
        
        if(input[0] != ':' || input.size() < 2){
            processExpression(input);
            std::cout << "\n";
        }
        else if(input[1] == 'q')
            keepLooping = false;
        else if(input[1] == '?')
            printHelp();
        else {
            std::cout << "unknown command '" << input.substr(1, input.size() - 1)
            << "' (use ':?' for help)" << "\n";
        }
    }
    return 0;
}
