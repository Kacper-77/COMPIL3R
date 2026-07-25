#pragma once

#include "token.h"
#include <string>

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
    Token Number();

    std::string source;
    size_t position = 0;

    int line = 1;
    int column = 1;
};
