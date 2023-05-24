#include "Lexer.hpp"

Lexer::Lexer(std::string filePath) : Line(0), peek(' '), lastToken(nullptr) {
    Line = 1;
    peek = ' ';
    fStream = std::ifstream(filePath);
    if (!fStream.is_open()) {
        std::cerr << "Failed to open " + filePath << std::endl;
        exit(1);
    }
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
            Line++;
            if (requiresSemicolon()) {
                return std::make_shared<Token>(TokenEnum::Semicolon);
            }
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
        } else if (lexeme == "for") {
            return std::make_shared<Token>(TokenEnum::For);
        } else if (lexeme == "break") {
            return std::make_shared<Token>(TokenEnum::Break);
        } else if (lexeme == "continue") {
            return std::make_shared<Token>(TokenEnum::Continue);
        } else if (lexeme == "func") {
            return std::make_shared<Token>(TokenEnum::Func);
        } else if (lexeme == "return") {
            return std::make_shared<Token>(TokenEnum::Return);
        } else if (lexeme == "true") {
            return std::make_shared<Word>(lexeme, TokenEnum::True);
        } else if (lexeme == "false") {
            return std::make_shared<Word>(lexeme, TokenEnum::False);
        }

        return std::make_shared<Word>(lexeme, TokenEnum::ID);
    }

    if (peek == '"') {
        std::string stringLiteral{};
        readChar();

        while (peek != '"') {
            if (peek == '\\') {
                readChar();
                switch (peek) {
                case 'n':
                    peek = '\n';
                    break;
                case 't':
                    peek = '\t';
                    break;
                case 'b':
                    peek = '\b';
                    break;
                case 'f':
                    peek = '\f';
                    break;
                case 'r':
                    peek = '\r';
                    break;
                case '"':
                    peek = '\"';
                    break;
                case '\\':
                    peek = '\\';
                    break;
                default:
                    std::cout << "ERROR UNDEFINED: escape sequence \\" << peek
                              << std::endl;
                    return std::make_shared<Token>(TokenEnum::NOTDEFINED);
                    ;
                }
            }
            stringLiteral += peek;
            readChar();
        }
        readChar();
        return std::make_shared<String>(stringLiteral);
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
        if (peek == '=') {
            readChar();
            return std::make_shared<Word>("+=", TokenEnum::AddAssignment);
        } else {
            return std::make_shared<Word>("+", TokenEnum::Plus);
        }
    case '-':
        if (peek == '=') {
            readChar();
            return std::make_shared<Word>("-=", TokenEnum::SubAssignment);
        } else {
            return std::make_shared<Word>("-", TokenEnum::Minus);
        }
    case '*':
        if (peek == '=') {
            readChar();
            return std::make_shared<Word>("*=", TokenEnum::MulAssignment);
        } else {
            return std::make_shared<Word>("*", TokenEnum::Asterisk);
        }
    case '/':
        if (peek == '=') {
            readChar();
            return std::make_shared<Word>("/=", TokenEnum::DivAssignment);
        } else {
            return std::make_shared<Word>("/", TokenEnum::Slash);
        }
    case '%':
        if (peek == '=') {
            readChar();
            return std::make_shared<Word>("/=", TokenEnum::RemAssignment);
        } else {
            return std::make_shared<Word>("/", TokenEnum::Percent);
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
    case ',':
        return std::make_shared<Word>(",", TokenEnum::Comma);
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
    case TokenEnum::Break:
    case TokenEnum::Return:
    case TokenEnum::Continue:
        return true;
    default:
        return false;
    }
}
