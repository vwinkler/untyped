#include "Parser.h"
#include <assert.h>
#include <sstream>
#include <algorithm>

SyntaxError::SyntaxError(size_t character, char receivedCharacter, std::vector<char> expectedCharacters)
 : std::runtime_error(createMessage(character, receivedCharacter, expectedCharacters)),
      mCharacter(character), mReceivedCharacter(receivedCharacter), mExpectedCharacters(std::move(expectedCharacters))
{
}

SyntaxError::SyntaxError(size_t character, char receivedCharacter, const std::string& expectation)
 : std::runtime_error(createMessage(character, receivedCharacter, expectation)),
      mCharacter(character), mReceivedCharacter(receivedCharacter)
{
}


std::string SyntaxError::createMessage(size_t character, char receivedCharacter, const std::vector<char>& expectedCharacter)
{
    std::stringstream msg;
    for(auto it = expectedCharacter.begin(); it != expectedCharacter.end(); ++it) {
        msg << "'" << *it << "'";
        if(it + 1 != expectedCharacter.end() && it + 2 != expectedCharacter.end())
            msg << ", ";
        else if (it + 1 != expectedCharacter.end())
            msg << " or ";
    }
    return createMessage(character, receivedCharacter, msg.str());
}

std::string SyntaxError::createMessage(size_t character, char receivedCharacter, const std::string& expectation)
{
    std::stringstream msg;
    msg << "syntax error at " << character << "; expected "
    << expectation
    << ", but received '" << receivedCharacter << "'";
    return msg.str();
}

Parser::Parser(std::istream& in)
 : mIn(in)
{
}

Term Parser::parseTerm()
{
    Term result('\0');
    char c = mIn.peek();
    switch(c) {
    case '(': { // Application
        mIn.get();
        Term leftTerm = parseTerm();
        expect(' ');
        Term rightTerm = parseTerm();
        expect(')');
        result = Term(std::move(leftTerm), std::move(rightTerm));
        break;
    }
    case '\\': { // Abstraction
        mIn.get();
        char argument = mIn.get();
        expect('.');
        Term trunk = parseTerm();
        result = Term(argument, std::move(trunk));
        break;
    }
    default: { // Variable
        if(c < 'a' || c >= 'z') {
            SyntaxError error(mIn.gcount(), c, "alphabetic character");
            throw error;
        }
        char variable = c;
        mIn.get();
        result = Term(variable);
        break;
    }
    }
    return result;
}

void Parser::expect(char expectedChar)
{
    expect(std::vector<char>({expectedChar}));
}

void Parser::expect(std::vector<char> expectedChars)
{
    char c = mIn.get();
    auto it = std::find(expectedChars.begin(), expectedChars.end(), c);
    if(it == expectedChars.end()) {
        SyntaxError error(mIn.gcount(), c, std::move(expectedChars));
        throw error;
    }
}

Term Parser::buildSyntaxTree()
{
    return parseTerm();
}
