#pragma once

#include <memory>
#include <string>

#include "scope.h"
#include "../ast/ast.h"

class SemanticAnalyzer {
public:
    void Analyze(ProgramNode& program);

private:
    /*
     * Program / declarations
     */

    void AnalyzeProgram(ProgramNode& node);

    void AnalyzeFunction(FunctionNode& node);

    void AnalyzeVariableDeclaration(
        VariableDeclarationNode& node
    );
    
    /*
     * Statements
     */

    void AnalyzeBlock(BlockNode& node);

    void AnalyzeStatement(ASTNode& node);

    void AnalyzeExpressionStatement(
        ExpressionStatementNode& node
    );

    void AnalyzeReturn(
        ReturnStatementNode& node
    );

    void AnalyzeIf(
        IfStatementNode& node
    );

    void AnalyzeWhile(
        WhileStatementNode& node
    );

    void AnalyzeFor(
        ForStatementNode& node
    );

    void AnalyzeBreak(
        BreakStatementNode& node
    );

    void AnalyzeContinue(
        ContinueStatementNode& node
    );
    
    /*
     * Expressions
     */

    ValueType AnalyzeExpression(ASTNode& node);

    ValueType AnalyzeNumber(
        NumberNode& node
    );

    ValueType AnalyzeBoolean(
        BooleanNode& node
    );

    ValueType AnalyzeIdentifier(
        IdentifierNode& node
    );

    ValueType AnalyzeUnary(
        UnaryExpressionNode& node
    );

    ValueType AnalyzeBinary(
        BinaryExpressionNode& node
    );

    ValueType AnalyzeAssignment(
        AssignmentExpressionNode& node
    );

    ValueType AnalyzeCall(
        CallExpressionNode& node
    );

    /*
     * Functions / symbols
     */

    void DeclareFunction(FunctionNode& node);

    void DeclareVariable(
        const std::string& name,
        ValueType type,
        bool isConst,
        SourceLocation location
    );

    Symbol* Resolve(
        const std::string& name,
        SourceLocation location
    );

    void EnterScope();
    void ExitScope();

    ValueType ToValueType(TokenType type);

    void RequireType(
        ValueType actual,
        ValueType expected,
        SourceLocation location
    );

    void RequireSameType(
        ValueType left,
        ValueType right,
        SourceLocation location
    );

    [[noreturn]]
    void Error(
        const std::string& message,
        SourceLocation location
    );

private:
    std::unique_ptr<Scope> globalScope;

    Scope* currentScope = nullptr;

    FunctionNode* currentFunction = nullptr;

    int loopDepth = 0;
};
