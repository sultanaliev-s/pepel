#pragma once
#include "../lexer/Token.hpp"
#include "../lexer/TokenEnum.hpp"
#include "Expression.hpp"
#include "Statement.hpp"

class Set : public Statement {
   public:
    std::shared_ptr<Token> Id;
    std::unique_ptr<Expression> Expr;

    Set(std::shared_ptr<Token> id, std::unique_ptr<Expression> expression)
        : Id(id), Expr(std::move(expression)) {
    }

    llvm::Value* codegen() override {
    }
    llvm::Value* Accept(NodeVisitor* visitor) override {
        return visitor->Visit(this);
    }
};
