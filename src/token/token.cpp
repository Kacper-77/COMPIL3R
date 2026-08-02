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
