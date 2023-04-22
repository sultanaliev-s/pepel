#pragma once
#include "Statement.hpp"

class ContinueStmt : public Statement {
   public:
    llvm::Value* Accept(NodeVisitor* visitor) override {
        return visitor->Visit(this);
    }

    std::string ToString() override {
        return "Continue;";
    }
};
