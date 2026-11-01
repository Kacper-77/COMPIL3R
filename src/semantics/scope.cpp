#include "scope.h"
#include "semantics/symbol.h"

Scope::Scope(Scope* parent) : parent{parent} {}

Scope* Scope::GetParent() const {
    return parent;
}

bool Scope::Declare(const Symbol& symbol) {
    Symbol* current = LookupCurrent(symbol.name);

    if (symbol.name.empty()) return false;
    if (current != nullptr) return false;

    symbols.emplace(symbol.name, symbol);
    return true;
}

Symbol* Scope::Lookup(const std::string& name) {
    if (name.empty()) return nullptr;

    Scope* scope = this;

    while (scope != nullptr) {
        Symbol* symbol = scope->LookupCurrent(name);
        if (symbol != nullptr) return symbol;

        scope = scope->parent;
    }
    return nullptr;
}

const Symbol* Scope::Lookup(const std::string& name) const {
    if (name.empty()) return nullptr;

    const Scope* scope = this;

    while (scope != nullptr) {
        const Symbol* symbol = scope->LookupCurrent(name);
        if (symbol != nullptr) return symbol;

        scope = scope->parent;
    }
    return nullptr;
}

Symbol* Scope::LookupCurrent(const std::string& name) {
    if (!name.empty()) {
        auto it = symbols.find(name);
        if (it != symbols.end()) return &it->second;
    }

    return nullptr;
}

const Symbol* Scope::LookupCurrent(const std::string& name) const {
    if (!name.empty()) {
        auto it = symbols.find(name);
        if (it != symbols.end()) return &it->second;
    }

    return nullptr;
}
