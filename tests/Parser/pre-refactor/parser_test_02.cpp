#include <iostream>
#include "lexer.h"
#include "token.h"
#include "parser.h"

/* PASSED AS EXPECTED */

int main() {
    const std::string source = R"(
        int add(int a, int b) {
            int result = a + b;

            if (result > 10) {
                return result;
            } else {
                return 0;
            }
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
