#include "Token.hpp"

Token::Token(TokenEnum type) : Type(type) {
}

std::string Token::ToString() {
    return "<" + TokenEnumToString(Type) + ">";
}
