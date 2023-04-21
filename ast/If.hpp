#pragma once
#include "../codegen/NodeVisitor.hpp"
#include "BlockStmt.hpp"
#include "Expression.hpp"

class If : public Statement {
   public:
    std::unique_ptr<Expression> Condition;
    std::unique_ptr<BlockStmt> Block;
    std::unique_ptr<Statement> Else;

    If(std::unique_ptr<Expression> condition, std::unique_ptr<BlockStmt> block,
        std::unique_ptr<Statement> elseStmt)
        : Condition(std::move(condition)),
          Block(std::move(block)),
          Else(std::move(elseStmt)) {
    }

    llvm::Value* Accept(NodeVisitor* visitor) override {
        return visitor->Visit(this);
    }

    std::string ToString() override {
        std::string elseStr = "";
        if (Else != nullptr) {
            elseStr = Else->ToString();
        }
        return "If{" + Condition->ToString() + Block->ToString() + elseStr +
               "}";
    }
};
