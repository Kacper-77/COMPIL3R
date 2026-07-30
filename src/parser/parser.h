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
    const Token& Previous() const;

    const Token& Advance();

    bool IsAtEnd() const;

    bool Check(TokenType type) const;
    bool Match(TokenType type);

    const Token& Consume(TokenType type);
};
