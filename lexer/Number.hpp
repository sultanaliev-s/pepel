#pragma once

#include "Token.hpp"
#include "TokenEnum.hpp"

class Number : public Token {
   public:
    const int Value;

    Number(int value);

    std::string ToString() override;
};
