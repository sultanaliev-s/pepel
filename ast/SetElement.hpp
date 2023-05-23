#pragma once
#include "../lexer/TokenEnum.hpp"
#include "../lexer/Word.hpp"
#include "Expression.hpp"
#include "Statement.hpp"

class SetElement : public Statement {
   public:
    std::shared_ptr<Word> Id;
    std::shared_ptr<Expression> Index;
    std::unique_ptr<Expression> Expr;

    SetElement(std::shared_ptr<Word> id, std::unique_ptr<Expression> index,
        std::unique_ptr<Expression> expression)
        : Id(id), Index(std::move(index)), Expr(std::move(expression)) {
    }

    llvm::Value* Accept(NodeVisitor* visitor) override {
        return visitor->Visit(this);
    }

    std::string ToString() override {
        return "SetElem{" + Id->ToString() + "[" + Index->ToString() + "]" +
               Expr->ToString() + "}";
    }
};
