#pragma once
#include "Operation.hpp"

class Logical : public Operation {
   public:
    std::unique_ptr<Expression> Left;
    std::unique_ptr<Expression> Right;

    Logical(std::shared_ptr<Token> op, std::unique_ptr<Expression> left,
        std::unique_ptr<Expression> right)
        : Operation(op), Left(std::move(left)), Right(std::move(right)) {
    }

    llvm::Value* Accept(NodeVisitor* visitor) override {
        return visitor->Visit(this);
    }

    std::string ToString() override {
        return "(" + Left->ToString() + Op->ToString() + Right->ToString() +
               ")";
    }
};
