#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

#include "codegen/CodegenNodeVisitor.hpp"
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: pepel <file>" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];

    auto lastDir = filePath.find_last_of('/');
    auto lastDot = filePath.find_last_of('.');
    std::string fileName = filePath.substr(lastDir + 1, lastDot - lastDir - 1);

    std::cout << filePath << std::endl;

    auto lexer = std::make_shared<Lexer>(filePath);
    Parser parser(lexer);
    auto program = parser.Parse();
    std::cout << "Successfully parsed" << std::endl;
    std::cout << program->ToString() << std::endl;

    CodegenNodeVisitor codegen;
    codegen.Compile(program.get(), fileName);

    return 0;
}
