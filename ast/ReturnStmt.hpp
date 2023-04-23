#pragma once
#include "Expression.hpp"
#include "Statement.hpp"

class ReturnStmt : public Statement {
   public:
    std::unique_ptr<Expression> Expr;

    ReturnStmt(std::unique_ptr<Expression> expr) : Expr(std::move(expr)) {
    }

    llvm::Value* Accept(NodeVisitor* visitor) override {
        return visitor->Visit(this);
    }

    std::string ToString() override {
        std::string expr = "";
        if (Expr != nullptr) {
            expr = " " + Expr->ToString();
        }
        return "Return" + expr + ";";
    }
};
