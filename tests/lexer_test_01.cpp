#include <iostream>
#include "lexer.h"
#include "token.h"

const char* TokenName(TokenType type) {
    switch (type) {
        case TokenType::Int: return "Int";
        case TokenType::Return: return "Return";
        case TokenType::Void: return "Void";
        case TokenType::Bool: return "Bool";
        case TokenType::If: return "If";
        case TokenType::Else: return "Else";
        case TokenType::While: return "While";
        case TokenType::For: return "For";
        case TokenType::True: return "True";
        case TokenType::False: return "False";
        case TokenType::Const: return "Const";

        case TokenType::Identifier: return "Identifier";
        case TokenType::Number: return "Number";
        
        case TokenType::Plus: return "Plus";
        case TokenType::Minus: return "Minus";
        case TokenType::Star: return "Star";
        case TokenType::Slash: return "Slash";

        case TokenType::Assign: return "Assign";

        case TokenType::EqualEqual: return "EqualEqual";
        case TokenType::NotEqual: return "NotEqual";
        case TokenType::Not: return "Not";
        case TokenType::Less: return "Less";
        case TokenType::LessEqual: return "LessEqual";
        case TokenType::Greater: return "Greater";
        case TokenType::GreaterEqual: return "GreaterEqual";

        case TokenType::LParen: return "LParen";
        case TokenType::RParen: return "RParen";
        case TokenType::LBrace: return "LBrace";
        case TokenType::RBrace: return "RBrace";
        case TokenType::Semicolon: return "Semicolon";
        case TokenType::Comma: return "Comma";

        case TokenType::Invalid: return "Invalid";
        case TokenType::EndOfFile: return "EOF";
    }
    return "Unknown";
}


int main() {
    const std::string source = R"(
        const int main() {
            int x = 10;
            int y = 20;

            if (x <= y) {
                return x + y;
            }

            while (x != 0) {
                x = x - 1;
            }

            bool flag = true;

            @
        }
    )";


    Lexer lexer(source);

    while (true) {
        Token token = lexer.NextToken();

        std::cout
            << TokenName(token.type)
            << " | \""
            << token.text
            << "\" | "
            << token.line
            << ":"
            << token.column
            << " | length="
            << token.length
            << '\n';

        if (token.type == TokenType::EndOfFile)
            break;
    }
    return 0;
}

