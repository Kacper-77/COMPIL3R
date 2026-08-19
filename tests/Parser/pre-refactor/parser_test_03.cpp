#include <iostream>
#include "lexer.h"
#include "token.h"
#include "parser.h"

/* PASSED AS EXPECTED */

int main() {
const std::string source = R"(
    int main() {
        int x = 0;

        for (int i = 0; i < 10; i = i + 1) {
            while (x < 5) {
                x = x + 1;

                if (x == 3) {
                    continue;
                }

                if (x == 4) {
                    break;
                }
            }
        }

        return x;
    }
)";

    Lexer lexer(source);

    std::vector<Token> tokens;

    while (true) {
        Token token = lexer.NextToken();
        tokens.push_back(token);

        if (token.type == TokenType::EndOfFile)
            break;
    }

    try {
        Parser parser(tokens);
        auto ast = parser.Parse();

        std::cout << "Parsing successful!\n";
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
