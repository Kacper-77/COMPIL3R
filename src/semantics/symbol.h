#pragma once

#include <string>
#include <vector>

#include "value_type.h"

enum class SymbolKind {
    Variable,
    Function
};

struct Symbol {
    std::string name;
    SymbolKind kind;

    ValueType type;

    bool isConst = false;

    std::vector<ValueType> parameterTypes;
};
