#pragma once
#include "../lexer/Token.hpp"
#include "Expression.hpp"

class Operation : public Expression {
   public:
    std::shared_ptr<Token> Op;

    Operation(std::shared_ptr<Token> op) : Op(op) {
    }
};
