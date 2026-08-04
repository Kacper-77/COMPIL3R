#include "parser.h"
#include "ast.h"
#include "token.h"
#include <memory>
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens)
    : tokens{tokens} {}

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

/* PARSE */

std::unique_ptr<ASTNode> Parser::Parse() {
    auto program = std::make_unique<ProgramNode>();

    while (!IsAtEnd()) {
        auto node = ParseDeclaration();

        if (node)
            program->declarations.push_back(std::move(node));
    }
    return program;
}

std::unique_ptr<ASTNode> Parser::ParseDeclaration() {
    return nullptr;
}

/* Functions / Blocks */

std::unique_ptr<ASTNode> Parser::ParseFunction() {
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::ParseBlock() {
    return nullptr;
}

/* Statements */

std::unique_ptr<ASTNode> Parser::ParseStatement() {
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::ParseVariableDeclaration() {
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::ParseReturnStatement() {
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::ParseIfStatement() {
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::ParseWhileStatement() {
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::ParseForStatement() {
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::ParseBreakStatement() {
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::ParseContinueStatement() {
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::ParseExpressionStatement() {
    return nullptr;
}

/* Expressions */

std::unique_ptr<ASTNode> Parser::ParseExpression() {
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::ParseAssignment() {
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::ParseEquality() {
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::ParseComparison() {
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::ParseTerm() {
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::ParseFactor() {
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::ParseUnary() {
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::ParsePrimary() {
    return nullptr;
}
