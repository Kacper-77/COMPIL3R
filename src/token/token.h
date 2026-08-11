#pragma once

#include <string>

enum class TokenType {
    // keywords
    Int,
    Void,
    Bool,

    If,
    Else,
    While,
    For,
    Continue,
    Break,

    Return,

    True,
    False,

    Const,

    // identifiers / literals
    Identifier,
    Number,

    // operators
    Plus,
    Minus,
    Star,
    Slash,

    Assign,
    Not,

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
    size_t length = 0;
};

const char* TokenName(TokenType type);
