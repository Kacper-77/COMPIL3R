#include "semantic_analyzer.h"
#include "SemanticError.h"
#include "scope.h"

#include <cassert>

void SemanticAnalyzer::RequireType(
        ValueType actual,
        ValueType expected,
        SourceLocation location
) {
    if (actual == expected) return;

    Error(
        std::string("Type mismatch: expected '") +
        ValueTypeName(expected) +
        "', got '" +
        ValueTypeName(actual) +
        "'",
        location
    );
}

void SemanticAnalyzer::RequireSameType(        
        ValueType left,
        ValueType right,
        SourceLocation location
) {
   if (left == right) return;

    Error(
        std::string("Type mismatch: left operand has type '") +
        ValueTypeName(left) +
        "', right operand has type '" +
        ValueTypeName(right) +
        "'",
        location
    );
}

void SemanticAnalyzer::EnterScope() {
    auto newScope = std::make_unique<Scope>(currentScope);

    scopes.push_back(std::move(newScope));
    currentScope = newScope.get();
}

void SemanticAnalyzer::ExitScope() {
    if (currentScope == nullptr) return;
    assert(currentScope->GetParent() != nullptr);

    currentScope = currentScope->GetParent();
}

void SemanticAnalyzer::Error(const std::string& message, 
                             SourceLocation location) {
    throw SemanticError(message, location);
}
