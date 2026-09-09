#pragma once

#include <string>
#include <unordered_map>

#include "symbol.h"

class Scope {
public:
    explicit Scope(Scope* parent = nullptr);

    bool Declare(const Symbol& symbol);

    Symbol* Lookup(const std::string& name);
    const Symbol* Lookup(const std::string& name) const;

    Symbol* LookupCurrent(const std::string& name);
    const Symbol* LookupCurrent(const std::string& name) const;

private:
    Scope* parent;

    std::unordered_map<std::string, Symbol> symbols;
};
