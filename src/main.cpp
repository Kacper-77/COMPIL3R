#include <iostream>
#include "lexer.h"

int main() {
    Lexer lexer("   abc");

    lexer.SkipWhitespace();
    std::cout << lexer.Peek() << '\n';
    std::cout << lexer.PeekNext() << '\n';

    std::cout << lexer.Advance() << '\n';

    std::cout << lexer.Peek() << '\n';
    std::cout << lexer.PeekNext() << '\n';

    return 0;
}
