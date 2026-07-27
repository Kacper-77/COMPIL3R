#include "lexer.h"

namespace {

    bool IsIdentifierChar(const char c) {
        return (c >= 'a' && c <= 'z') ||
               (c >= 'A' && c <= 'Z') ||
               (c >= '0' && c <= '9') ||
               c == '_';
    }

};

Lexer::Lexer(const std::string& source) : source{source} {}

char Lexer::Peek() const {
    if (IsAtEnd()) return '\0';
    return (char)source[position];
}

char Lexer::PeekNext() const {
    if (IsAtEnd()) return '\0';
    if (position + 1 >= source.size()) return '\0';
    return (char)source[position+1];
}

char Lexer::Advance() {
    if (IsAtEnd()) return '\0';
    char c = Peek();

    position++;

    if (c == '\n') {
        line++;
        column = 1;
    } else if (c == '\t') {
        column += 4;
    } else {
        column++;
    }

    return c;
}

void Lexer::SkipWhitespace() {
    while (Peek() == ' ' || Peek() == '\n' || Peek() == '\t') {
        Advance(); 
    }
}

bool Lexer::IsAtEnd() const {
    return position >= source.size();
}

Token Lexer::ReadNumber() {
    int startLine = line;
    int startColumn = column;
    size_t length = 0;
    std::string txt = "";

    while (std::isdigit(static_cast<unsigned char>(Peek()))) {
        txt.push_back(Peek());
        Advance();
        length++;
    }

    return Token {
            TokenType::Number,
            txt,
            startLine,
            startColumn,
            length
        };
}

Token Lexer::ReadIdentifier() {
    int startLine = line;
    int startColumn = column;
    size_t length = 0;
    std::string txt = "";

    while (IsIdentifierChar(Peek())) {
        txt.push_back(Peek());
        Advance();
        length++;
    }

    if (Keywords::tokenTypeMap.count(txt)) {
        return Token {
            Keywords::tokenTypeMap.at(txt),
            txt,
            startLine,
            startColumn,
            length
        };
    } else {
        return Token {
            TokenType::Identifier,
            txt,
            startLine,
            startColumn,
            length
        };
    }
}

Token Lexer::NextToken() {
    
}
