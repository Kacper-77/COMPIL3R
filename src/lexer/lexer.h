#pragma once

#include "token.h"
#include <string>
#include <unordered_map>

namespace Keywords {

    const std::unordered_map<std::string, TokenType> tokenTypeMap{
        {"int", TokenType::Int},
        {"return",TokenType::Return}
    };

}

class Lexer {
public:
    Lexer(const std::string& source);

    Token NextToken();

// private:
    char Peek() const;
    char PeekNext() const ;
    char Advance();

    void SkipWhitespace();
    bool IsAtEnd() const;

    Token ReadIdentifier();
    Token ReadNumber();

    std::string source;
    size_t position = 0;

    int line = 1;
    int column = 1;
};
