#include "lexer.h"

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
