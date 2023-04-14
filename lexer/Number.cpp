#include "Number.hpp"

Number::Number(int value) : Token(TokenEnum::Num), Value(value) {
}

std::string Number::ToString() {
    return "<" + std::to_string(Value) + ";" + TokenEnumToString(Type) + ">";
}
