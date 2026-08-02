#include "parser.h"
#include "token.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens)
    : tokens{tokens} {}


std::unique_ptr<ASTNode> Parser::Parse() {
    return nullptr;
}

const Token& Parser::Peek() const {
    return tokens[current];
}

const Token& Parser::Previous() const {
    return tokens[current - 1];
}

const Token& Parser::Advance() {
    if (!IsAtEnd()) current++;
    return Previous();
}


bool Parser::IsAtEnd() const {
    return Peek().type == TokenType::EndOfFile; 
}


bool Parser::Check(TokenType type) const {
    return Peek().type == type;
}

bool Parser::Match(TokenType type) {
    if (Check(type)) {
        Advance();
        return true;
    }
    return false;
}

const Token& Parser::Consume(const TokenType type) {
    if (!Check(type)) {
        const Token& t = Peek();
        throw std::runtime_error(
            "Parser error at: " + std::to_string(t.line) + ":" + std::to_string(t.column) +  
            "\nExpected token '" +
            std::string(TokenName(type)) +
            "', got '" +
            std::string(TokenName(t.type)) +
            "'."
        );
    }
    return Advance();
}
