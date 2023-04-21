#pragma once
#include <vector>

#include "../codegen/NodeVisitor.hpp"
#include "Statement.hpp"

class BlockStmt : public Statement {
   public:
    std::vector<std::unique_ptr<Statement>> Statements;

    BlockStmt() : Statements(0) {
    }

    llvm::Value* Accept(NodeVisitor* visitor) override {
        return visitor->Visit(this);
    }

    std::string ToString() override {
        std::string str = "Block{\n";
        for (size_t i = 0; i < Statements.size(); i++) {
            str += Statements[i]->ToString() + "\n";
        }
        str += "}";
        return str;
    }
};
