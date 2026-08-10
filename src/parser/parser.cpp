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
    const Token& t = Peek();
    auto expr = ParseExpression();
    Consume(TokenType::Semicolon);

    return std::make_unique<ExpressionStatementNode>(
        std::move(expr),
        SourceLocation{t.line, t.column}
    );
}

/* Expressions */

std::unique_ptr<ASTNode> Parser::ParseExpression() {
    return ParseAssignment();
}

std::unique_ptr<ASTNode> Parser::ParseAssignment() {
    auto target = ParseEquality();

    if (Match(TokenType::Assign)) {
        const Token& op = Previous();
        auto value = ParseAssignment();

        return std::make_unique<AssignmentExpressionNode>(
            std::move(target),
            std::move(value),
            SourceLocation{op.line, op.column}
        );
    }
    return target;    
}

std::unique_ptr<ASTNode> Parser::ParseEquality() {
    auto expr = ParseComparison();

    while (Match(TokenType::EqualEqual) || Match(TokenType::NotEqual)) {
        const Token& op = Previous();
        auto right = ParseComparison();

        expr = std::make_unique<BinaryExpressionNode>(
            op.type,
            std::move(expr),
            std::move(right),
            SourceLocation{op.line, op.column}
        );
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::ParseComparison() {
    auto expr = ParseTerm();

    while (Match(TokenType::Less) ||
        Match(TokenType::LessEqual) ||
        Match(TokenType::Greater) ||
        Match(TokenType::GreaterEqual)) {
        
        const Token& op = Previous();
        auto right = ParseTerm();

        expr = std::make_unique<BinaryExpressionNode>(
            op.type,
            std::move(expr),
            std::move(right),
            SourceLocation{op.line, op.column}
        );
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::ParseTerm() {
    auto expr = ParseFactor();

    while (Match(TokenType::Plus) || Match(TokenType::Minus)) {
        const Token& op = Previous();
        auto right = ParseFactor();

        expr = std::make_unique<BinaryExpressionNode>(
            op.type,
            std::move(expr),
            std::move(right),
            SourceLocation{op.line, op.column}
        );
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::ParseFactor() {
    auto expr = ParseUnary();

    while (Match(TokenType::Star) || Match(TokenType::Slash)) {
        const Token& op = Previous();
        auto right = ParseUnary();

        expr = std::make_unique<BinaryExpressionNode>(
            op.type,
            std::move(expr),
            std::move(right),
            SourceLocation{op.line, op.column}
        );
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::ParseUnary() {
    if (Match(TokenType::Minus) || Match(TokenType::Not)) {
        const Token& op = Previous();
        auto operand = ParseUnary();

        return std::make_unique<UnaryExpressionNode>(
            op.type,
            std::move(operand),
            SourceLocation{op.line, op.column}
        );
    }
    return ParsePrimary();
}

std::unique_ptr<ASTNode> Parser::ParsePrimary() {
    if (Match(TokenType::Number)) {
        const Token& t = Previous();
        return std::make_unique<NumberNode>(
            std::stoi(t.text),
            SourceLocation{t.line, t.column}
        );
    }

    if (Match(TokenType::True) || Match(TokenType::False)) {
        const Token& t = Previous();
        return std::make_unique<BooleanNode>(
            t.type == TokenType::True,
            SourceLocation{t.line, t.column}
        );
    }

    if (Match(TokenType::Identifier)) {
        const Token& t = Previous();
        return std::make_unique<IdentifierNode>(
            t.text, SourceLocation{t.line, t.column}
        );
    }
    
    const Token& t = Peek();

    throw std::runtime_error(
        "Parser error at: " +
        std::to_string(t.line) + ":" +
        std::to_string(t.column) +
        "\nExpected primary expression, got '" +
        std::string(TokenName(t.type)) +
        "'."
    );
}
