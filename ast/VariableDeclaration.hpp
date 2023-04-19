#pragma once
#include "../lexer/Word.hpp"
#include "Expression.hpp"
#include "Statement.hpp"

class VariableDeclaration : public Statement {
   public:
    std::shared_ptr<Word> Type;
    std::shared_ptr<Word> Id;
    std::unique_ptr<Expression> Expr;

    VariableDeclaration(std::shared_ptr<Word> type, std::shared_ptr<Word> id,
        std::unique_ptr<Expression> expr)
        : Type(type), Id(id), Expr(std::move(expr)) {
    }

    llvm::Value* codegen() override;

    std::string ToString() override {
        return "VarDecl{" + Type->ToString() + " " + Id->ToString() +
               Expr->ToString() + "}";
    }
};