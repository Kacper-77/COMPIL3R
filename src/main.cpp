#include <cassert>
#include <iostream>
#include "scope.h"

int main() {
    std::cout << "### SCOPE TESTS ###\n";

    // 1. Basic declaration and lookup
    {
        Scope global;

        Symbol x;
        x.name = "x";
        x.kind = SymbolKind::Variable;
        x.type = ValueType::Int;
        x.isConst = false;

        assert(global.Declare(x));
        assert(global.LookupCurrent("x") != nullptr);
        assert(global.Lookup("x") != nullptr);
        assert(global.Lookup("missing") == nullptr);

        std::cout << "[PASS] Basic declaration and lookup\n";
    }

    // 2. Duplicate declaration
    {
        Scope global;

        Symbol x;
        x.name = "x";
        x.kind = SymbolKind::Variable;
        x.type = ValueType::Int;

        assert(global.Declare(x));
        assert(!global.Declare(x));

        std::cout << "[PASS] Duplicate declaration\n";
    }

    // 3. Empty symbol name
    {
        Scope global;

        Symbol empty;
        empty.name = "";

        assert(!global.Declare(empty));
        assert(global.Lookup("") == nullptr);

        std::cout << "[PASS] Empty symbol name\n";
    }

    // 4. Parent scope lookup
    {
        Scope global;

        Symbol x;
        x.name = "x";
        x.kind = SymbolKind::Variable;
        x.type = ValueType::Int;

        assert(global.Declare(x));

        Scope function(&global);
        assert(function.Lookup("x") != nullptr);
        assert(function.LookupCurrent("x") == nullptr);

        std::cout << "[PASS] Parent scope lookup\n";
    }

    // 5. Shadowing
    {
        Scope global;

        Symbol x;
        x.name = "x";
        x.kind = SymbolKind::Variable;
        x.type = ValueType::Int;

        assert(global.Declare(x));

        Scope child(&global);

        Symbol localX;
        localX.name = "x";
        localX.kind = SymbolKind::Variable;
        localX.type = ValueType::Bool;

        assert(child.Declare(localX));

        assert(child.LookupCurrent("x")->type == ValueType::Bool);
        assert(child.Lookup("x")->type == ValueType::Bool);
        assert(global.Lookup("x")->type == ValueType::Int);

        std::cout << "[PASS] Shadowing\n";
    }

    // 6. Nested scope lookup
    {
        Scope global;

        Symbol x;
        x.name = "x";
        x.kind = SymbolKind::Variable;
        x.type = ValueType::Int;

        assert(global.Declare(x));

        Scope function(&global);
        Scope block(&function);
        Scope innerBlock(&block);

        assert(innerBlock.Lookup("x") != nullptr);

        std::cout << "[PASS] Nested scope lookup\n";
    }

    // 7. Nearest declaration wins
    {
        Scope global;

        Symbol x;
        x.name = "x";
        x.kind = SymbolKind::Variable;
        x.type = ValueType::Int;
        assert(global.Declare(x));

        Scope function(&global);

        Symbol functionX;
        functionX.name = "x";
        functionX.kind = SymbolKind::Variable;
        functionX.type = ValueType::Bool;
        assert(function.Declare(functionX));

        Scope block(&function);

        assert(block.Lookup("x")->type == ValueType::Bool);

        std::cout << "[PASS] Nearest declaration wins\n";
    }

    // 8. Parent does not see child declarations
    {
        Scope global;
        Scope child(&global);

        Symbol local;
        local.name = "local";
        local.kind = SymbolKind::Variable;
        local.type = ValueType::Int;

        assert(child.Declare(local));

        assert(global.Lookup("local") == nullptr);
        assert(child.Lookup("local") != nullptr);

        std::cout << "[PASS] Parent cannot see child symbols\n";
    }

    // 9. Different symbols in different scopes
    {
        Scope global;
        Scope child(&global);

        Symbol a;
        a.name = "a";
        a.kind = SymbolKind::Variable;
        a.type = ValueType::Int;

        Symbol b;
        b.name = "b";
        b.kind = SymbolKind::Variable;
        b.type = ValueType::Bool;

        assert(global.Declare(a));
        assert(child.Declare(b));

        assert(child.Lookup("a") != nullptr);
        assert(child.Lookup("b") != nullptr);
        assert(global.Lookup("b") == nullptr);

        std::cout << "[PASS] Independent scope contents\n";
    }

    // 10. Const lookup
    {
        Scope global;

        Symbol x;
        x.name = "x";
        x.kind = SymbolKind::Variable;
        x.type = ValueType::Int;

        assert(global.Declare(x));

        const Scope& constGlobal = global;

        const Symbol* found = constGlobal.Lookup("x");

        assert(found != nullptr);
        assert(found->name == "x");
        assert(found->type == ValueType::Int);

        std::cout << "[PASS] Const lookup\n";
    }

    std::cout << "\nALL SCOPE TESTS PASSED!\n";

    return 0;
}
