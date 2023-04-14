#pragma once

#include "Token.hpp"
#include "TokenEnum.hpp"

class RealNum : public Token {
   public:
    const double Value;

    RealNum(double value);

    std::string ToString() override;
};