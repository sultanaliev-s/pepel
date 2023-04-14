#include "Lexer.hpp"

Lexer::Lexer(std::string filePath) {
    Line = 0;
    peek = ' ';
    fStream = std::ifstream(filePath);
    fStream >> std::noskipws;
    keywords = std::map<std::string, Token>();
}

std::shared_ptr<Token> Lexer::Scan() {
    for (readChar();; readChar()) {
        if (peek == ' ' || peek == '\t') {
            continue;
        } else if (peek == '\n') {
            Line++;
        } else {
            break;
        }
    }

    if (isdigit(peek) || peek == '.') {  // Number: [0-9.]+
        int value{0};
        while (isdigit(peek)) {
            value = value * 10 + peek - '0';
            readChar();
        }
        if (peek != '.') {
            return std::make_shared<Number>(value);
        }
        double real = value;
        double d = 10;
        readChar();
        while (isdigit(peek)) {
            real = real + charToInt(peek) / d;
            d *= 10;
            readChar();
        }

        return std::make_shared<RealNum>(real);
    }

    if (isalpha(peek)) {  // identifier: [a-zA-Z][a-zA-Z0-9]*
        std::string lexeme{};

        while (isalnum(peek)) {
            lexeme += peek;
            readChar();
        }
        return std::make_shared<Word>(lexeme, TokenEnum::ID);
    }

    switch (peek) {
        case '|':
            if (readChar('|')) {
                return std::make_shared<Word>("||", TokenEnum::Or);
            } else {
                return std::make_shared<Word>("|", TokenEnum::Pipe);
            }
        case '=':
            if (readChar('=')) {
                return std::make_shared<Word>("==", TokenEnum::Equal);
            } else {
                return std::make_shared<Word>("=", TokenEnum::Assign);
            }
    }

    if (peek == EOF) {
        return std::make_shared<Token>(TokenEnum::EndOfFile);
    }

    return std::make_shared<Token>(TokenEnum::NOTDEFINED);
}

bool Lexer::readChar(int ch) {
    readChar();
    if (peek != ch) {
        return false;
    }

    peek = ' ';
    return true;
}

void Lexer::readChar() {
    char x;
    fStream.get(x);
    peek = x;
    if (fStream.eof()) {
        peek = EOF;
    }
}

int Lexer::charToInt(char x) {
    return x - '0';
}
