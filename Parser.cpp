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
        msg << convertChar(*it);
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
    << ", but received " << convertChar(receivedCharacter);
    return msg.str();
}

std::string SyntaxError::convertChar(char c)
{
    std::stringstream msg;
    switch(c) {
    case std::char_traits<char>::eof():
        msg << "end of file";
        break;
    default:
        msg << "'" << c << "'";
        break;
    }
    return msg.str();
}

Parser::Parser(std::istream& in)
 : mIn(in), mCharCount(0)
{
}

Term Parser::parseTerm()
{
    Term result('\0');
    char c = peek();
    switch(c) {
    case '(': { // Application
        get();
        Term leftTerm = parseTerm();
        expect(' ');
        Term rightTerm = parseTerm();
        expect(')');
        result = Term(std::move(leftTerm), std::move(rightTerm));
        break;
    }
    case '\\': { // Abstraction
        get();
        char argument = get();
        expect('.');
        Term trunk = parseTerm();
        result = Term(argument, std::move(trunk));
        break;
    }
    default: { // Variable
        get();
        if(c < 'a' || c >= 'z') {
            SyntaxError error(mCharCount, c, "alphabetic character");
            throw error;
        }
        result = Term(c);
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
    char c = get();
    auto it = std::find(expectedChars.begin(), expectedChars.end(), c);
    if(it == expectedChars.end()) {
        SyntaxError error(mCharCount, c, std::move(expectedChars));
        throw error;
    }
}

Term Parser::buildSyntaxTree()
{
    Term result = parseTerm();
    expect(std::char_traits<char>::eof());
    return result;
}

char Parser::get()
{
    ++mCharCount;
    return mIn.get();
}

char Parser::peek()
{
    return mIn.peek();
}
