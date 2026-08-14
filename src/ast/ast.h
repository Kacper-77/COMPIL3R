#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "token.h"


struct SourceLocation {
    int line = 0;
    int column = 0;
};


enum class ASTNodeType {
    // root
    Program,
    Function,
    Parameter,
    Block,

    // expressions
    NumberLiteral,
    BooleanLiteral,
    Identifier,

    UnaryExpression,
    BinaryExpression,
    AssignmentExpression,
    CallExpression,

    // statements
    ExpressionStatement,
    VariableDeclaration,
    ReturnStatement,
    IfStatement,
    WhileStatement,
    ForStatement,
    BreakStatement,
    ContinueStatement,

    EmptyStatement
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

/* Literals / Identifiers */

class NumberNode : public ASTNode {
public:
    int value;

    NumberNode(int value, SourceLocation location = {})
        : ASTNode{ASTNodeType::NumberLiteral, location},
          value{value}
    {}
};


class BooleanNode : public ASTNode {
public:
    bool value;

    BooleanNode(bool value, SourceLocation location = {})
        : ASTNode{ASTNodeType::BooleanLiteral, location},
          value{value}
    {}
};


class IdentifierNode : public ASTNode {
public:
    std::string name;

    IdentifierNode(
        const std::string& name,
        SourceLocation location = {})
        : ASTNode{ASTNodeType::Identifier, location},
          name{name}
    {}
};


/* Expressions */

class UnaryExpressionNode : public ASTNode {
public:
    TokenType op;
    std::unique_ptr<ASTNode> operand;

    UnaryExpressionNode(
        TokenType op,
        std::unique_ptr<ASTNode> operand,
        SourceLocation location = {})
        : ASTNode{ASTNodeType::UnaryExpression, location},
          op{op},
          operand{std::move(operand)}
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
        SourceLocation location = {})
        : ASTNode{ASTNodeType::BinaryExpression, location},
          op{op},
          left{std::move(left)},
          right{std::move(right)}
    {}
};


class AssignmentExpressionNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> target;
    std::unique_ptr<ASTNode> value;

    AssignmentExpressionNode(
        std::unique_ptr<ASTNode> target,
        std::unique_ptr<ASTNode> value,
        SourceLocation location = {})
        : ASTNode{ASTNodeType::AssignmentExpression, location},
          target{std::move(target)},
          value{std::move(value)}
    {}
};


class CallExpressionNode : public ASTNode {
public:
    std::string callee;

    std::vector<std::unique_ptr<ASTNode>> arguments;

    CallExpressionNode(
        const std::string& callee,
        SourceLocation location = {})
        : ASTNode{ASTNodeType::CallExpression, location},
          callee{callee}
    {}
};

/* Statements */

class VariableDeclarationNode : public ASTNode {
public:
    TokenType type;
    std::string name;
    std::unique_ptr<ASTNode> initializer;
    bool isConst;

    VariableDeclarationNode(
        TokenType type,
        const std::string& name,
        std::unique_ptr<ASTNode> initializer,
        bool isConst,
        SourceLocation location = {})
        : ASTNode{ASTNodeType::VariableDeclaration, location},
          type{type},
          name{name},
          initializer{std::move(initializer)},
          isConst{isConst}
    {}
};


class ReturnStatementNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> expression;

    ReturnStatementNode(
        std::unique_ptr<ASTNode> expression,
        SourceLocation location = {})
        : ASTNode{ASTNodeType::ReturnStatement, location},
          expression{std::move(expression)}
    {}
};


class ExpressionStatementNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> expression;

    ExpressionStatementNode(
        std::unique_ptr<ASTNode> expression,
        SourceLocation location = {})
        : ASTNode{ASTNodeType::ExpressionStatement, location},
          expression{std::move(expression)}
    {}
};


class BlockNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> statements;

    BlockNode(SourceLocation location = {})
        : ASTNode{ASTNodeType::Block, location}
    {}
};


class IfStatementNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> thenBranch;
    std::unique_ptr<ASTNode> elseBranch;

    IfStatementNode(
        std::unique_ptr<ASTNode> condition,
        std::unique_ptr<ASTNode> thenBranch,
        std::unique_ptr<ASTNode> elseBranch,
        SourceLocation location = {})
        : ASTNode{ASTNodeType::IfStatement, location},
          condition{std::move(condition)},
          thenBranch{std::move(thenBranch)},
          elseBranch{std::move(elseBranch)}
    {}
};


class WhileStatementNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> body;

    WhileStatementNode(
        std::unique_ptr<ASTNode> condition,
        std::unique_ptr<ASTNode> body,
        SourceLocation location = {})
        : ASTNode{ASTNodeType::WhileStatement, location},
          condition{std::move(condition)},
          body{std::move(body)}
    {}
};


class ForStatementNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> initializer;
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> increment;
    std::unique_ptr<ASTNode> body;

    ForStatementNode(
        std::unique_ptr<ASTNode> initializer,
        std::unique_ptr<ASTNode> condition,
        std::unique_ptr<ASTNode> increment,
        std::unique_ptr<ASTNode> body,
        SourceLocation location = {})
        : ASTNode{ASTNodeType::ForStatement, location},
          initializer{std::move(initializer)},
          condition{std::move(condition)},
          increment{std::move(increment)},
          body{std::move(body)}
    {}
};


class BreakStatementNode : public ASTNode {
public:
    BreakStatementNode(SourceLocation location = {})
        : ASTNode{ASTNodeType::BreakStatement, location}
    {}
};


class ContinueStatementNode : public ASTNode {
public:
    ContinueStatementNode(SourceLocation location = {})
        : ASTNode{ASTNodeType::ContinueStatement, location}
    {}
};

/* Functions / Program */

class ParameterNode : public ASTNode {
public:
    TokenType type;
    std::string name;

    ParameterNode(
        TokenType type,
        const std::string& name,
        SourceLocation location = {})
        : ASTNode{ASTNodeType::Parameter, location},
          type{type},
          name{name}
    {}
};


class FunctionNode : public ASTNode {
public:
    TokenType type;
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> parameters;
    std::unique_ptr<ASTNode> body;

    FunctionNode(
        TokenType type,
        const std::string& name,
        std::vector<std::unique_ptr<ASTNode>> params,
        std::unique_ptr<ASTNode> body,
        SourceLocation location = {})
        : ASTNode{ASTNodeType::Function, location},
          type{type},
          name{name},
          parameters{std::move(params)},
          body{std::move(body)}
    {}
};


class ProgramNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> declarations;

    ProgramNode(SourceLocation location = {})
        : ASTNode{ASTNodeType::Program, location}
    {}
};
