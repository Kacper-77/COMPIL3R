#pragma once

#include <memory>
#include <string>
#include "token.h"


struct SourceLocation {
    int line = 0;
    int column = 0;
};


enum class ASTNodeType {
    Program,

    NumberLiteral,
    Identifier,

    BinaryExpression,

    VariableDeclaration,
    ReturnStatement,

    Block
};


class ASTNode {
public:
    ASTNode(ASTNodeType type, SourceLocation location = {})
        : type(type),
          location(location)
    {}

    virtual ~ASTNode() = default;

    ASTNodeType type;
    SourceLocation location;
};


class NumberNode : public ASTNode {
public:
    int value;

    NumberNode(int value, SourceLocation location = {})
        : ASTNode(ASTNodeType::NumberLiteral, location),
          value(value)
    {}
};


class IdentifierNode : public ASTNode {
public:
    std::string name;

    IdentifierNode(const std::string& name, SourceLocation location = {})
        : ASTNode(ASTNodeType::Identifier, location),
          name(name)
    {}
};


class BinaryExpressionNode : public ASTNode {
public:
    TokenType op;

    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;


    BinaryExpressionNode(
        TokenType op,
        std::unique_ptr<ASTNode> left,
        std::unique_ptr<ASTNode> right,
        SourceLocation location = {}
    )
        : ASTNode(ASTNodeType::BinaryExpression, location),
          op(op),
          left(std::move(left)),
          right(std::move(right))
    {}
};
