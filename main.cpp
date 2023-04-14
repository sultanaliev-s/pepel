#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

#include "lexer/Lexer.hpp"

int main() {
    Lexer lexer("samples/example.txt");
    for (auto t = lexer.Scan();; t = lexer.Scan()) {
        std::cout << t->ToString() << " ";

        if (t->Type == TokenEnum::EndOfFile) {
            break;
        }
    }

    return 0;
}
