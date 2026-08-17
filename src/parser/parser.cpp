#include "parser.h"
#include "ast.h"
#include "token.h"
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

Parser::Parser(const std::vector<Token>& tokens)
    : tokens{tokens} {}

const Token& Parser::Peek() const {
    return tokens[current];
}

const Token& Parser::PeekNext() const {
    return tokens[current + 1];
}

const Token& Parser::PeekNextNext() const {
    return tokens[current + 2];
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
        auto declaration = ParseDeclaration();
        program->declarations.push_back(std::move(declaration));
    }
    return program;
}

std::unique_ptr<ASTNode> Parser::ParseDeclaration() {
    const Token& t = Peek();

    if (Check(TokenType::Const))
        return ParseVariableDeclaration();

    switch (Peek().type) {
        case TokenType::Int:
        case TokenType::Bool:
        case TokenType::Void:
            if (PeekNext().type == TokenType::Identifier &&
                PeekNextNext().type == TokenType::LParen) {
                return ParseFunction();
            }

            return ParseVariableDeclaration();

        default:
            throw std::runtime_error(
                "Parser error at: " +
                std::to_string(t.line) + ":" +
                std::to_string(t.column) +
                "\nExpected declaration, got '" +
                std::string(TokenName(t.type)) +
                "'."
            );
    }
}

/* Functions / Blocks */

std::unique_ptr<ASTNode> Parser::ParseFunction() {
    const Token& t = Peek();

    TokenType funcType;
    if (Match(TokenType::Int)) funcType = TokenType::Int;
    else if (Match(TokenType::Bool)) funcType = TokenType::Bool;
    else if (Match(TokenType::Void)) funcType = TokenType::Void;
    else throw std::runtime_error("Parser error: Unknown type at: " 
                                  + std::to_string(t.line) + " " 
                                  + std::to_string(t.column));
    
    const Token& name = Consume(TokenType::Identifier);
    const std::string funcName = name.text;
    Consume(TokenType::LParen);
    
    std::vector<std::unique_ptr<ASTNode>> params;

    while (!Check(TokenType::RParen)) {
        const Token& t2 = Peek();
        TokenType paramType;
        
        if (Match(TokenType::Int)) paramType = TokenType::Int;
        else if (Match(TokenType::Bool)) paramType = TokenType::Bool;
        else throw std::runtime_error("Parser error: Unknown type at: " 
                                  + std::to_string(t2.line) + ":" 
                                  + std::to_string(t2.column));

        const Token& name = Consume(TokenType::Identifier);
        const std::string paramName = name.text;
        
        auto param = std::make_unique<ParameterNode>(
            paramType,
            paramName
        );

        params.push_back(std::move(param));

        if (Peek().type == TokenType::Comma) {
            Consume(TokenType::Comma);
        } else {
            break;
        }
    }
    
    Consume(TokenType::RParen);
    auto body = ParseBlock();
    
    return std::make_unique<FunctionNode>(
        funcType,
        funcName,
        std::move(params),
        std::move(body),
        SourceLocation{t.line, t.column}
    );
}

std::unique_ptr<ASTNode> Parser::ParseBlock() {
    const Token& t = Peek();
    Consume(TokenType::LBrace);

    auto block = std::make_unique<BlockNode>(SourceLocation{t.line, t.column});

    while (!Check(TokenType::RBrace) && !IsAtEnd()) {
        auto statement = ParseStatement();
        block->statements.push_back(std::move(statement));
    }
    Consume(TokenType::RBrace);

    return block;
}

/* Statements */

std::unique_ptr<ASTNode> Parser::ParseStatement() {
    switch (Peek().type) {
        case TokenType::Int:
        case TokenType::Bool:
        case TokenType::Const: {
            auto declaration = ParseVariableDeclaration();
            Consume(TokenType::Semicolon);
            return declaration;
        }

        case TokenType::If:
            return ParseIfStatement();

        case TokenType::While:
            return ParseWhileStatement();

        case TokenType::For:
            return ParseForStatement();

        case TokenType::Return:
            return ParseReturnStatement();

        case TokenType::Break:
            return ParseBreakStatement();

        case TokenType::Continue:
            return ParseContinueStatement();

        case TokenType::LBrace:
            return ParseBlock();

        default:
            return ParseExpressionStatement();
    }
}

std::unique_ptr<ASTNode> Parser::ParseVariableDeclaration() {
    const Token& t = Peek();
    
    std::unique_ptr<ASTNode> initilizer;
    bool isConst = Match(TokenType::Const);

    TokenType type;

    if (Match(TokenType::Int)) 
        type = TokenType::Int;
    else if (Match(TokenType::Bool))
        type = TokenType::Bool;
    else
        throw std::runtime_error("Unknown type at: " 
                            + std::to_string(t.line) 
                            + ":" + std::to_string(t.column));
    auto name = Peek();
    Consume(TokenType::Identifier);
    
    if (!Check(TokenType::Semicolon)) {
        Consume(TokenType::Assign);
        initilizer = ParseExpression();
    }

    return std::make_unique<VariableDeclarationNode>(
        type,
        name.text,
        std::move(initilizer),
        isConst,
        SourceLocation{t.line, t.column}
    );
}

std::unique_ptr<ASTNode> Parser::ParseReturnStatement() {
    const Token& t = Peek();

    Consume(TokenType::Return);

    std::unique_ptr<ASTNode> expr;
    
    if (Check(TokenType::Semicolon)) 
        expr = nullptr;
    else
        expr = ParseExpression();

    Consume(TokenType::Semicolon);

    return std::make_unique<ReturnStatementNode>(
        std::move(expr),
        SourceLocation{t.line, t.column}
    );
}

std::unique_ptr<ASTNode> Parser::ParseIfStatement() {
    const Token& t = Peek();

    Consume(TokenType::If);
    Consume(TokenType::LParen);

    auto condition = ParseExpression();
    Consume(TokenType::RParen);

    auto thenB = ParseBlock();
    std::unique_ptr<ASTNode> elseB;

    if (Match(TokenType::Else)) {
        const Token& token = Peek();

        if (token.type == TokenType::If) {
            elseB = ParseIfStatement();
        } else {
            elseB = ParseBlock();
        }
    }

    return std::make_unique<IfStatementNode>(
        std::move(condition),
        std::move(thenB),
        std::move(elseB),
        SourceLocation{t.line, t.column}
    );
}

std::unique_ptr<ASTNode> Parser::ParseWhileStatement() {
    const Token& t = Peek();

    Consume(TokenType::While);
    Consume(TokenType::LParen);

    auto condition = ParseExpression();
    Consume(TokenType::RParen);

    auto body = ParseBlock();

    return std::make_unique<WhileStatementNode>(
        std::move(condition),
        std::move(body),
        SourceLocation{t.line, t.column}
    );
}

std::unique_ptr<ASTNode> Parser::ParseForStatement() {
    const Token& t = Peek();

    Consume(TokenType::For);
    Consume(TokenType::LParen);

    auto initilizer = ParseVariableDeclaration();
    Consume(TokenType::Semicolon);

    auto condition = ParseExpression();
    Consume(TokenType::Semicolon);

    auto increment = ParseExpression();
    Consume(TokenType::RParen);

    auto body = ParseBlock();

    return std::make_unique<ForStatementNode>(
        std::move(initilizer),
        std::move(condition),
        std::move(increment),
        std::move(body),
        SourceLocation{t.line, t.column}
    );
}

std::unique_ptr<ASTNode> Parser::ParseBreakStatement() {
    const Token& t = Peek();

    Consume(TokenType::Break);
    Consume(TokenType::Semicolon);

    return std::make_unique<BreakStatementNode>(
        SourceLocation{t.line, t.column}
    );
}

std::unique_ptr<ASTNode> Parser::ParseContinueStatement() {
    const Token& t = Peek();

    Consume(TokenType::Continue);
    Consume(TokenType::Semicolon);

    return std::make_unique<ContinueStatementNode>(
        SourceLocation{t.line, t.column}
    );
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
