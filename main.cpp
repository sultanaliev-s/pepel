#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"

int main() {
    std::string filePath = "samples/main.txt";
    std::cout << filePath << std::endl;
    auto lexer = std::make_shared<Lexer>(filePath);
    Parser parser(lexer);
    auto res = parser.Parse();

    std::cout << res->ToString() << std::endl;

    return 0;
}
