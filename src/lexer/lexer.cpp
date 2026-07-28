#include "lexer.h"
#include "token.h"

namespace {

   bool IsIdentifierStart(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
    }

    bool IsIdentifierChar(char c) {
        return IsIdentifierStart(c) ||
               (c >= '0' && c <= '9');
    }

}

Lexer::Lexer(const std::string& source) : source{source} {}

Token Lexer::MakeInvalidToken() {
    int startLine = line;
    int startColumn = column;
    char invalid = Advance();

    return Token{
        TokenType::Invalid,
        std::string(1, invalid),
        startLine,
        startColumn,
        1
    };
}


char Lexer::Peek() const {
    if (IsAtEnd()) return '\0';
    return (char)source[position];
}

char Lexer::PeekNext() const {
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
    auto keyword = Keywords::tokenTypeMap.find(txt);

    if (keyword != Keywords::tokenTypeMap.end()) {
        return Token{
            keyword->second,
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
   SkipWhitespace();

    if (IsAtEnd()) return Token{TokenType::EndOfFile, "", line, column, 0};

    if (std::isdigit(static_cast<unsigned char>(Peek())))
        return ReadNumber();

    if (IsIdentifierStart(Peek()))
        return ReadIdentifier();

    switch (Peek()) {
        case '+':
            Advance();
            return Token{TokenType::Plus, "+", line, column - 1, 1};

        case '-':
            Advance();
            return Token{TokenType::Minus, "-", line, column - 1, 1};

        case '*':
            Advance();
            return Token{TokenType::Star, "*", line, column - 1, 1};

        case '/':
            Advance();
            return Token{TokenType::Slash, "/", line, column - 1, 1};

        case '=':
            if (PeekNext() == '=') {
                int startLine = line;
                int startColumn = column;
                Advance();
                Advance();
                return Token{TokenType::EqualEqual, "==", startLine, startColumn, 2};
            } else {
                int startLine = line;
                int startColumn = column;
                Advance();
                return Token{TokenType::Assign, "=", startLine, startColumn, 1};
            }

        case '!':
            if (PeekNext() == '=') {
                int startLine = line;
                int startColumn = column;
                Advance();
                Advance();
                return Token{TokenType::NotEqual, "!=", startLine, startColumn, 2};
            } else {
                int startLine = line;
                int startColumn = column;
                Advance();
                return Token{TokenType::Not, "!", startLine, startColumn, 1};
            }
            break;

        case '<':
            if (PeekNext() == '=') {
                int startLine = line;
                int startColumn = column;
                Advance();
                Advance();
                return Token{TokenType::LessEqual, "<=", startLine, startColumn, 2};
            } else {
                int startLine = line;
                int startColumn = column;
                Advance();
                return Token{TokenType::Less, "<", startLine, startColumn, 1};
            }

        case '>':
            if (PeekNext() == '=') {
                int startLine = line;
                int startColumn = column;
                Advance();
                Advance();
                return Token{TokenType::GreaterEqual, ">=", startLine, startColumn, 2};
            } else {
                int startLine = line;
                int startColumn = column;
                Advance();
                return Token{TokenType::Greater, ">", startLine, startColumn, 1};
            }

        case '(':
            Advance();
            return Token{TokenType::LParen, "(", line, column - 1, 1};

        case ')':
            Advance();
            return Token{TokenType::RParen, ")", line, column - 1, 1};

        case '{':
            Advance();
            return Token{TokenType::LBrace, "{", line, column - 1, 1};

        case '}':
            Advance();
            return Token{TokenType::RBrace, "}", line, column - 1, 1};

        case ';':
            Advance();
            return Token{TokenType::Semicolon, ";", line, column - 1, 1};

        case ',':
            Advance();
            return Token{TokenType::Comma, ",", line, column - 1, 1};

        default:
            break;
    }
    return MakeInvalidToken();
}
