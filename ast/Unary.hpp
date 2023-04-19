#pragma once
#include "Operation.hpp"

class Unary : public Operation {
   public:
    std::unique_ptr<Expression> Expr;

    Unary(std::shared_ptr<Token> op, std::unique_ptr<Expression> expr)
        : Operation(op), Expr(std::move(expr)) {
    }

    llvm::Value* Accept(NodeVisitor* visitor) override {
        return visitor->Visit(this);
    }

    std::string ToString() override {
        return "(" + Op->ToString() + Expr->ToString() + ")";
    }
};
