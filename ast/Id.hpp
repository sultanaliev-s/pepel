#pragma once
#include "../lexer/Token.hpp"
#include "Expression.hpp"

class Id : public Expression {
   public:
    std::shared_ptr<Token> Tok;

    Id(std::shared_ptr<Token> token) : Tok(std::move(token)) {
    }

    llvm::Value* codegen() override;

    std::string ToString() override {
        return Tok->ToString();
    }
};
