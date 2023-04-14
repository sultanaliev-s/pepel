#include "Word.hpp"

Word::Word(std::string lexeme, TokenEnum type) : Token(type), Lexeme(lexeme) {
}

std::string Word::ToString() {
    return "<" + Lexeme + ";" + TokenEnumToString(Type) + ">";
}
