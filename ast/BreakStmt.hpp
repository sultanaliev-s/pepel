#pragma once
#include "Statement.hpp"

class BreakStmt : public Statement {
   public:
    llvm::Value* Accept(NodeVisitor* visitor) override {
        return visitor->Visit(this);
    }

    std::string ToString() override {
        return "Break;";
    }
};
