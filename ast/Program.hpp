#pragma once
#include <vector>

#include "Statement.hpp"

class Program : public Node {
   public:
    std::vector<std::unique_ptr<Statement>> Statements;

    Program() : Statements(0) {
    }

    llvm::Value* codegen() override {
    }

    std::string ToString() override;
};
