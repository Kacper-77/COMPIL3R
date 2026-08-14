#pragma once

#include <vector>
#include <memory>

#include "token.h"
#include "ast.h"

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);

    std::unique_ptr<ASTNode> Parse();


private:
    const std::vector<Token>& tokens;
    size_t current = 0;


    const Token& Peek() const;
    const Token& PeekNext() const;
    const Token& PeekNextNext() const;
    const Token& Previous() const;

    const Token& Advance();

    bool IsAtEnd() const;

    bool Check(TokenType type) const;
    bool Match(TokenType type);

    const Token& Consume(const TokenType type);

    // entry
    std::unique_ptr<ASTNode> ParseDeclaration();

    // functions / blocks
    std::unique_ptr<ASTNode> ParseFunction();
    std::unique_ptr<ASTNode> ParseBlock();

    // statements
    std::unique_ptr<ASTNode> ParseStatement();
    std::unique_ptr<ASTNode> ParseVariableDeclaration();
    std::unique_ptr<ASTNode> ParseReturnStatement();
    std::unique_ptr<ASTNode> ParseIfStatement();
    std::unique_ptr<ASTNode> ParseWhileStatement();
    std::unique_ptr<ASTNode> ParseForStatement();
    std::unique_ptr<ASTNode> ParseBreakStatement();
    std::unique_ptr<ASTNode> ParseContinueStatement();
    std::unique_ptr<ASTNode> ParseExpressionStatement();

    // expressions
    std::unique_ptr<ASTNode> ParseExpression();
    std::unique_ptr<ASTNode> ParseAssignment();
    std::unique_ptr<ASTNode> ParseEquality();
    std::unique_ptr<ASTNode> ParseComparison();
    std::unique_ptr<ASTNode> ParseTerm();
    std::unique_ptr<ASTNode> ParseFactor();
    std::unique_ptr<ASTNode> ParseUnary();
    std::unique_ptr<ASTNode> ParsePrimary();
};
