#pragma once
#include "../lexer/Number.hpp"
#include "../lexer/Word.hpp"
#include "VariableDeclaration.hpp"

class ArrayDeclaration : public VariableDeclaration {
   public:
    const int Size;

    ArrayDeclaration(std::shared_ptr<Word> type, const int size,
        std::shared_ptr<Word> id, std::unique_ptr<Expression> expr)
        : VariableDeclaration(type, id, std::move(expr)), Size(size) {
    }

    llvm::Value* Accept(NodeVisitor* visitor) override {
        return visitor->Visit(this);
    }

    std::string ToString() override {
        std::string expr = "";
        if (Expr != nullptr) {
            expr = Expr->ToString();
        }
        return "ArrayDecl{" + Type->ToString() + "[" + std::to_string(Size) +
               "] " + Id->ToString() + expr + "}";
    }
};
