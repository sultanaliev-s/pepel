#pragma once
#include "Expression.hpp"
#include "Statement.hpp"

class ExpressionStmt : public Statement {
   public:
    std::unique_ptr<Expression> Expr;

    ExpressionStmt(std::unique_ptr<Expression> expression)
        : Expr(std::move(expression)) {
    }

    llvm::Value* Accept(NodeVisitor* visitor) override {
        return visitor->Visit(this);
    }

    std::string ToString() override {
        return "ExprStmt{" + Expr->ToString() + "}";
    }
};
