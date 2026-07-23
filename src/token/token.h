#pragma once

#include <string>

enum class TokenType {
    // keywords
    Int,
    Return,

    // identifiers / literals
    Identifier,
    Number,

    // operators
    Plus,
    Minus,
    Star,
    Slash,

    Assign,

    // comparison
    EqualEqual,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,

    // punctuation
    LParen,
    RParen,
    LBrace,
    RBrace,
    Semicolon,
    Comma,

    EndOfFile,
    Invalid
};

struct Token {
    TokenType type = TokenType::EndOfFile;
    std::string text;
    int line = 0;
    int column = 0;
};
