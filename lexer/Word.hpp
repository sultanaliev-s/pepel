#pragma once
#include "Token.hpp"
#include "TokenEnum.hpp"

class Word : public Token {
   public:
    const std::string Lexeme;

    Word(std::string lexeme, TokenEnum type);

    std::string ToString() override;
};