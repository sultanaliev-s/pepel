#include "RealNum.hpp"

RealNum::RealNum(double value) : Token(TokenEnum::Real), Value(value) {
}

std::string RealNum::ToString() {
    return "<" + std::to_string(Value) + ";" + TokenEnumToString(Type) + ">";
}
