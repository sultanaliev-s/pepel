#pragma once
#include "../lexer/TokenEnum.hpp"
#include "../lexer/Word.hpp"
#include "Expression.hpp"
#include "Statement.hpp"

class Set : public Statement {
   public:
    std::shared_ptr<Word> Id;
    std::unique_ptr<Expression> Expr;

    Set(std::shared_ptr<Word> id, std::unique_ptr<Expression> expression)
        : Id(id), Expr(std::move(expression)) {
    }

    llvm::Value* Accept(NodeVisitor* visitor) override {
        return visitor->Visit(this);
    }

    std::string ToString() override {
        return "Set{" + Id->ToString() + " " + Expr->ToString() + "}";
    }
};
