#pragma once

#include <stdexcept>
#include <string>
#include "../ast/ast.h"

class SemanticError : std::runtime_error {
public:
    SemanticError(const std::string& message, SourceLocation location);
};
