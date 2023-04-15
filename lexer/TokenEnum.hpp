#pragma once
#include <string>

enum class TokenEnum {
    NOTDEFINED,
    Ampersand,
    And,
    Assign,
    Asterisk,
    Comma,
    Dot,
    Else,
    EndOfFile,
    Equal,
    False,
    For,
    Greater,
    GreaterEqual,
    ID,
    If,
    Less,
    LessEqual,
    Minus,
    NotEqual,
    Num,
    Or,
    Pipe,
    Plus,
    Real,
    Semicolon,
    Slash,
    True,
    AddAssignment,
    SubAssignment,
};

inline std::string TokenEnumToString(TokenEnum e) {
    switch (e) {
        case TokenEnum::ID:
            return "Identification";
        case TokenEnum::Num:
            return "Number";
        case TokenEnum::Real:
            return "Real";
        case TokenEnum::EndOfFile:
            return "End of file";
        case TokenEnum::Equal:
            return "Equal";
        case TokenEnum::Assign:
            return "Assign";
        default:
            return "Not registered";
    }
}
