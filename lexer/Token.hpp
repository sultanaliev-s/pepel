#pragma once
#include <string>

#include "TokenEnum.hpp"

class Token {
   public:
    TokenEnum Type;

    Token(TokenEnum type);

    virtual std::string ToString();
};
