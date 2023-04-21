#include "Lexer.hpp"

Lexer::Lexer(std::string filePath) : Line(0), peek(' '), lastToken(nullptr) {
    Line = 0;
    peek = ' ';
    fStream = std::ifstream(filePath);
    fStream >> std::noskipws;
}

std::shared_ptr<Token> Lexer::Scan() {
    lastToken = scan();
    return lastToken;
}

std::shared_ptr<Token> Lexer::scan() {
    for (;; readChar()) {
        if (peek == ' ' || peek == '\t') {
            continue;
        } else if (peek == '\n') {
            if (requiresSemicolon()) {
                return std::make_shared<Token>(TokenEnum::Semicolon);
            }
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

        if (lexeme == "if") {
            return std::make_shared<Token>(TokenEnum::If);
        } else if (lexeme == "else") {
            return std::make_shared<Token>(TokenEnum::Else);
        } else if (lexeme == "true") {
            return std::make_shared<Word>(lexeme, TokenEnum::True);
        } else if (lexeme == "false") {
            return std::make_shared<Word>(lexeme, TokenEnum::False);
        }

        return std::make_shared<Word>(lexeme, TokenEnum::ID);
    }

    auto cur = peek;
    readChar();
    switch (cur) {
    case '|':
        if (peek == '|') {
            readChar();
            return std::make_shared<Word>("||", TokenEnum::Or);
        } else {
            return std::make_shared<Word>("|", TokenEnum::Pipe);
        }
    case '&':
        if (peek == '&') {
            readChar();
            return std::make_shared<Word>("&&", TokenEnum::And);
        } else {
            return std::make_shared<Word>("&", TokenEnum::Ampersand);
        }
    case '=':
        if (peek == '=') {
            readChar();
            return std::make_shared<Word>("==", TokenEnum::Equal);
        } else {
            return std::make_shared<Word>("=", TokenEnum::Assign);
        }
    case '+':
        if (peek == '+') {
            readChar();
            return std::make_shared<Word>("+=", TokenEnum::AddAssignment);
        } else {
            return std::make_shared<Word>("+", TokenEnum::Plus);
        }
    case '-':
        if (peek == '-') {
            readChar();
            return std::make_shared<Word>("-=", TokenEnum::SubAssignment);
        } else {
            return std::make_shared<Word>("-", TokenEnum::Minus);
        }
    case '*':
        if (peek == '*') {
            readChar();
            return std::make_shared<Word>("*=", TokenEnum::MulAssignment);
        } else {
            return std::make_shared<Word>("*", TokenEnum::Asterisk);
        }
    case '/':
        if (peek == '/') {
            readChar();
            return std::make_shared<Word>("/=", TokenEnum::DivAssignment);
        } else {
            return std::make_shared<Word>("/", TokenEnum::Slash);
        }
    case '>':
        if (peek == '=') {
            readChar();
            return std::make_shared<Word>(">=", TokenEnum::GreaterEqual);
        } else {
            return std::make_shared<Word>(">", TokenEnum::Greater);
        }
    case '<':
        if (peek == '=') {
            readChar();
            return std::make_shared<Word>("<=", TokenEnum::LessEqual);
        } else {
            return std::make_shared<Word>("<", TokenEnum::Less);
        }
    case '!':
        if (peek == '=') {
            readChar();
            return std::make_shared<Word>("!=", TokenEnum::NotEqual);
        } else {
            return std::make_shared<Word>("!", TokenEnum::Bang);
        }
    case '(':
        return std::make_shared<Word>("(", TokenEnum::LParen);
    case ')':
        return std::make_shared<Word>(")", TokenEnum::RParen);
    case '{':
        return std::make_shared<Word>("{", TokenEnum::LBrace);
    case '}':
        return std::make_shared<Word>("}", TokenEnum::RBrace);
    case '[':
        return std::make_shared<Word>("[", TokenEnum::LBracket);
    case ']':
        return std::make_shared<Word>("]", TokenEnum::RBracket);
    case ';':
        return std::make_shared<Word>(";", TokenEnum::Semicolon);
    }

    if (cur == EOF) {
        return std::make_shared<Token>(TokenEnum::EndOfFile);
    }
    std::cout << "ERROR UNDEFINED: " << peek << std::endl;
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

bool Lexer::requiresSemicolon() {
    if (lastToken == nullptr) {
        return false;
    }
    switch (lastToken->Type) {
    case TokenEnum::ID:
    case TokenEnum::Num:
    case TokenEnum::Real:
    case TokenEnum::True:
    case TokenEnum::False:
    case TokenEnum::RParen:
    case TokenEnum::RBrace:
        return true;
    default:
        return false;
    }
}
