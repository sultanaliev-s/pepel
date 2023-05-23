#pragma once
#include "../lexer/TokenEnum.hpp"
#include "../lexer/Word.hpp"
#include "Expression.hpp"
#include "Statement.hpp"

class AccessElement : public Expression {
   public:
    std::string Id;
    std::shared_ptr<Expression> Index;

    AccessElement(std::string id, std::unique_ptr<Expression> index)
        : Id(id), Index(std::move(index)) {
    }

    llvm::Value* Accept(NodeVisitor* visitor) override {
        return visitor->Visit(this);
    }

    std::string ToString() override {
        return "AccessElement{" + Id + "[" + Index->ToString() + "]}";
    }
};
