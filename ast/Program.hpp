#pragma once
#include <vector>

#include "Statement.hpp"

class Program : public Node {
   public:
    std::vector<std::unique_ptr<Statement>> Statements;

    Program() : Statements(0) {
    }

    llvm::Value* Accept(NodeVisitor* visitor) override {
        return visitor->Visit(this);
    }

    std::string ToString() override;
};
