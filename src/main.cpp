#include <iostream>
#include "lexer.h"
#include "token.h"
#include "parser.h"


int main() {
    const std::string source = R"(
        int main() {
            int x = 10;
            int y = 20;

            if (x <= y) {
                return x + y;
            }

            while (x != 0) {
                x = x - 1;
            }

            bool flag = true;
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
