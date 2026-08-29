#include <iostream>
#include "lexer.h"
#include "token.h"
#include "parser.h"

const std::string valid1 = R"(
    const int global = 10;

    // comment

    int add(int a, int b) { // comment
        int result = a + b * 2;
        return result;
    }

    bool test(int value) {
        bool flag = true;

        if (value >= 10) {
            flag = false;
        } else if (value != 5) {
            flag = true;
        } else { // comment
            flag = false;
        }

        while (value > 0) {
            value = value - 1;

            if (value == 5) {
                continue;
            }

            // comment
            // comment 

            if (value == 2) {
                break;
            }
        }

        for (int i = 0; i < 10; i = i + 1) {
            value = value + i;
        }

        return flag;
    }

    void main() {
        int x = add();
        bool result = result();

        if (!result) {
            return;
        }

        return;
    }
)";

const std::string valid2 = R"(
    int main() {
        const int x = 10;
        bool flag = !false;

        int y = -x + 20 / 2;

        if (x < y && flag) {
            y = y - 1;
        }

        while (y != 0) {
            y = y - 1;
        }

        return y;
    }
)";

const std::string invalid1 = R"(
    int main() {
        int x = 10
        bool flag = true;

        return x;
    }
)";

const std::string invalid2 = R"(
    int main(int x, bool flag {
        if (x > 10) {
            return x;
        }

        while (x != 0) {
            x = x - 1;
        }

        return x;
    }
)";

void TestParser(const std::string& source, bool shouldPass) {
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

        if (shouldPass)
            std::cout << "[PASS] expected success\n";
        else
            std::cout << "[FAIL] parser accepted invalid source\n";
    }
    catch (const std::exception& e) {
        if (!shouldPass)
            std::cout << "[PASS] expected parser error: "
                      << e.what() << '\n';
        else
            std::cout << "[FAIL] unexpected parser error: "
                      << e.what() << '\n';
    }
}

int main() {
    TestParser(valid1, true);
    std::cout << "TEST: VALID1\n";

    TestParser(valid2, true);
    std::cout << "TEST: VALID2\n";

    TestParser(invalid1, false);
    std::cout << "TEST: INVALID1\n";

    TestParser(invalid2, false);
    std::cout << "TEST: INVALID2\n";
    return 0;
}
