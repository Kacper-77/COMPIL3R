#pragma once

enum class ValueType {
    Int,
    Bool,
    Void
};

const char* ValueTypeName(ValueType type);
