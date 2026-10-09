#include "value_type.h"

const char* ValueTypeName(ValueType type)
{
    switch (type) {
        case ValueType::Int:
            return "int";

        case ValueType::Bool:
            return "bool";

        case ValueType::Void:
            return "void";
    }

    return "unknown";
}
