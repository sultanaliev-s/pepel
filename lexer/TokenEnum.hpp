#pragma once
#include <string>

enum class TokenEnum {
    NOTDEFINED,

    ID,
    Num,
    Real,
    EndOfFile,

    // Punctuation
    LParen,
    RParen,
    LBrace,
    RBrace,
    Comma,
    Dot,
    Semicolon,
    LBracket,
    RBracket,

    // Operators
    AddAssignment,
    Ampersand,
    And,
    Assign,
    Asterisk,
    Bang,
    Equal,
    Greater,
    GreaterEqual,
    Less,
    LessEqual,
    Minus,
    NotEqual,
    Or,
    Pipe,
    Plus,
    Slash,
    SubAssignment,
    MulAssignment,
    DivAssignment,

    // Keywords
    Else,
    False,
    For,
    Func,
    If,
    True,
    Break,
    Continue,
    Return,
};

inline std::string TokenEnumToString(TokenEnum e) {
    switch (e) {
    case TokenEnum::ID:
        return "Identifier";
    case TokenEnum::Num:
        return "Number";
    case TokenEnum::Real:
        return "Real";
    case TokenEnum::LParen:
        return "Left Parenthesis";
    case TokenEnum::RParen:
        return "Right Parenthesis";
    case TokenEnum::LBrace:
        return "Left Brace";
    case TokenEnum::RBrace:
        return "Right Brace";
    case TokenEnum::LBracket:
        return "Left Bracket";
    case TokenEnum::RBracket:
        return "Right Bracket";
    case TokenEnum::Comma:
        return "Comma";
    case TokenEnum::Dot:
        return "Dot";
    case TokenEnum::EndOfFile:
        return "End of File";
    case TokenEnum::Semicolon:
        return "Semicolon";
    case TokenEnum::AddAssignment:
        return "Addition Assignment";
    case TokenEnum::Ampersand:
        return "Ampersand";
    case TokenEnum::And:
        return "Logical And";
    case TokenEnum::Assign:
        return "Assignment";
    case TokenEnum::Asterisk:
        return "Multiplication";
    case TokenEnum::Bang:
        return "Logical Not";
    case TokenEnum::Equal:
        return "Equal";
    case TokenEnum::Greater:
        return "Greater Than";
    case TokenEnum::GreaterEqual:
        return "Greater Than or Equal";
    case TokenEnum::Less:
        return "Less Than";
    case TokenEnum::LessEqual:
        return "Less Than or Equal";
    case TokenEnum::Minus:
        return "Subtraction";
    case TokenEnum::NotEqual:
        return "Not Equal";
    case TokenEnum::Or:
        return "Logical Or";
    case TokenEnum::Pipe:
        return "Pipe";
    case TokenEnum::Plus:
        return "Addition";
    case TokenEnum::Slash:
        return "Division";
    case TokenEnum::SubAssignment:
        return "Subtraction Assignment";
    case TokenEnum::MulAssignment:
        return "Multiplication Assignment";
    case TokenEnum::DivAssignment:
        return "Division Assignment";
    case TokenEnum::Else:
        return "Else";
    case TokenEnum::False:
        return "False";
    case TokenEnum::For:
        return "For";
    case TokenEnum::Func:
        return "Function";
    case TokenEnum::Break:
        return "Break";
    case TokenEnum::Continue:
        return "Continue";
    case TokenEnum::Return:
        return "Return";
    case TokenEnum::If:
        return "If";
    case TokenEnum::True:
        return "True";
    default:
        return "Not Registered";
    }
}
