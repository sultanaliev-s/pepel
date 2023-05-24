#include "String.hpp"

String::String(std::string literal)
    : Token(TokenEnum::String), Literal(literal) {
}

std::string String::ToString() {
    return "<" + Literal + ";" + TokenEnumToString(Type) + ">";
}
