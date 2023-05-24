#pragma once
#include "Token.hpp"
#include "TokenEnum.hpp"

class String : public Token {
   public:
    const std::string Literal;

    String(std::string literal);

    std::string ToString() override;
};
