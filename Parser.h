#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
#include <stdexcept>
#include "Term.h"

class SyntaxError : public std::runtime_error {
public:
    const size_t mCharacter;
    const char mReceivedCharacter;
    const std::vector<char> mExpectedCharacters;
    
    SyntaxError(size_t character, char receivedCharacter, std::vector<char> expectedCharacters);
    SyntaxError(size_t character, char receivedCharacter, const std::string& expectation);
    
private:
    static std::string createMessage(size_t character, char receivedCharacter, const std::vector<char>& expectedCharacter);
    static std::string createMessage(size_t character, char receivedCharacter, const std::string& expectation);
};

class Parser
{
    std::istream& mIn;
    
    Term parseTerm();
    void expect(char expectedChar);
    void expect(std::vector<char> expectedChars);
    
public:
    Parser(std::istream& in);
    Term buildSyntaxTree();
};

#endif // PARSER_H
