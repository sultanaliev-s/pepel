#pragma once
#include "../lexer/Number.hpp"
#include "Expression.hpp"
#include "Statement.hpp"

class Constant : public Expression {
   public:
    std::shared_ptr<Token> Tok;

    Constant(std::shared_ptr<Token> token) : Tok(std::move(token)) {
    }

    llvm::Value* codegen() override;

    std::string ToString() override {
        return Tok->ToString();
    }
};
