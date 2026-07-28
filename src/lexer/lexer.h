#pragma once

#include "token.h"
#include <string>
#include <unordered_map>

namespace Keywords {

    const std::unordered_map<std::string, TokenType> tokenTypeMap{
        // types
        {"int", TokenType::Int},
        {"void", TokenType::Void},
        {"bool", TokenType::Bool},

        // control flow
        {"if", TokenType::If},
        {"else", TokenType::Else},
        {"while", TokenType::While},
        {"for", TokenType::For},

        // functions
        {"return", TokenType::Return},

        // logical
        {"true", TokenType::True},
        {"false", TokenType::False},

        {"const", TokenType::Const}
    };

}

class Lexer {
public:
    Lexer(const std::string& source);

    Token NextToken();

private:
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

    Token MakeInvalidToken();
};
