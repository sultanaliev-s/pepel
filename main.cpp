#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

#include "codegen/CodegenNodeVisitor.hpp"
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"

int main() {
    std::string filePath = "samples/main.txt";
    std::cout << filePath << std::endl;
    auto lexer = std::make_shared<Lexer>(filePath);
    Parser parser(lexer);
    auto program = parser.Parse();

    CodegenNodeVisitor codegen;
    codegen.Compile(program.get());

    std::cout << program->ToString() << std::endl;

    return 0;
}
