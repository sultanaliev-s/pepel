#pragma once
#include "Expression.hpp"

class Call : public Expression {
   public:
    std::string FuncName;
    std::vector<std::unique_ptr<Expression>> Args;

    Call(std::string funcName, std::vector<std::unique_ptr<Expression>> args)
        : FuncName(std::move(funcName)), Args(std::move(args)) {
    }

    llvm::Value* Accept(NodeVisitor* visitor) override {
        return visitor->Visit(this);
    }

    std::string ToString() override {
        return "Call " + FuncName;
    }
};
