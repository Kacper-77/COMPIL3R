#include "SemanticError.h"
#include "ast.h"

SemanticError::SemanticError(const std::string& message, SourceLocation location) 
    : std::runtime_error(
        "Semantic error at " +
        std::to_string(location.line) + ":" +
        std::to_string(location.column) + ": " +
        message
      ) 
{}
