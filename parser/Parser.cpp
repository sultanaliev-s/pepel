#include "Parser.hpp"

class VariableDeclaration : public Statement {
   public:
    std::shared_ptr<Word> Type;
    std::shared_ptr<Word> Id;
    std::unique_ptr<Expression> Expr;

    VariableDeclaration(std::shared_ptr<Word> type, std::shared_ptr<Word> id,
        std::unique_ptr<Expression> expr)
        : Type(type), Id(id), Expr(std::move(expr)) {
    }

    std::string ToString() override {
        return "VarDecl{" + Type->ToString() + " " + Id->ToString() +
               Expr->ToString() + "}";
    }
};

class Constant : public Expression {
   public:
    std::shared_ptr<Token> Tok;

    Constant(std::shared_ptr<Token> token) : Tok(std::move(token)) {
    }

    std::string ToString() override {
        return Tok->ToString();
    }
};

class Id : public Expression {
   public:
    std::shared_ptr<Token> Tok;

    Id(std::shared_ptr<Token> token) : Tok(std::move(token)) {
    }

    std::string ToString() override {
        return Tok->ToString();
    }
};

class Operation : public Expression {
   public:
    std::shared_ptr<Token> Op;

    Operation(std::shared_ptr<Token> op) : Op(op) {
    }

    std::string ToString() override {
        return Op->ToString();
    }
};

class Arithmetic : public Operation {
   public:
    std::unique_ptr<Expression> Left;
    std::unique_ptr<Expression> Right;

    Arithmetic(std::shared_ptr<Token> op, std::unique_ptr<Expression> left,
        std::unique_ptr<Expression> right)
        : Operation(op), Left(std::move(left)), Right(std::move(right)) {
    }

    std::string ToString() override {
        return "(" + Left->ToString() + Op->ToString() + Right->ToString() +
               ")";
    }
};

class Unary : public Operation {
   public:
    std::unique_ptr<Expression> Expr;

    Unary(std::shared_ptr<Token> op, std::unique_ptr<Expression> expr)
        : Operation(op), Expr(std::move(expr)) {
    }

    std::string ToString() override {
        return "(" + Op->ToString() + Expr->ToString() + ")";
    }
};

std::string Program::ToString() {
    std::string str = "Program{\n";
    for (size_t i = 0; i < Statements.size(); i++) {
        str += Statements[i]->ToString() + "\n";
    }
    str += "}";
    return str;
}

Parser::Parser(std::shared_ptr<Lexer> _lexer) : lexer(_lexer) {
    next();
    registerBasicTypes();
    registerKeywords();
}

std::unique_ptr<Program> Parser::Parse() {
    return std::move(program());
}

void Parser::next() {
    curToken = lexer->Scan();
}

bool Parser::match(TokenEnum token) {
    if (curToken->Type == token) {
        next();
        return true;
    } else {
        error("syntax error");
    }
}

void Parser::error(std::string message) {
    std::cout << message;
    std::abort();
}

std::unique_ptr<Program> Parser::program() {
    auto prog = std::make_unique<Program>();
    while (curToken->Type != TokenEnum::EndOfFile) {
        auto stmt = statement();
        prog->Statements.push_back(std::move(stmt));
    }
    return std::move(prog);
}

std::unique_ptr<Statement> Parser::statement() {
    auto stmt = assign();
    return std::move(stmt);
}

std::unique_ptr<Statement> Parser::assign() {
    if (curToken->Type == TokenEnum::ID) {
        auto word = std::static_pointer_cast<Word>(curToken);
        if (isBasicType(word->Lexeme)) {
            next();
            auto id = std::static_pointer_cast<Word>(curToken);
            next();
            if (curToken->Type == TokenEnum::Assign) {
                next();
                auto expr = expression();
                match(TokenEnum::Semicolon);
                return std::make_unique<VariableDeclaration>(
                    word, id, std::move(expr));
            } else if (curToken->Type == TokenEnum::Semicolon) {
                next();
                return std::make_unique<VariableDeclaration>(word, id, nullptr);
            }
        }
        std::cout << "assign func end" << std::endl;
    }

    switch (curToken->Type) {
    case TokenEnum::ID:
        identifier();
        break;

    default:
        break;
    }
}

std::unique_ptr<Expression> Parser::expression() {
    auto x = term();
    while (curToken->Type == TokenEnum::Plus ||
           curToken->Type == TokenEnum::Minus) {
        auto tok = curToken;
        next();
        x = std::make_unique<Arithmetic>(tok, std::move(x), term());
    }
    return x;
}

std::unique_ptr<Expression> Parser::term() {
    auto x = unary();
    while (curToken->Type == TokenEnum::Asterisk ||
           curToken->Type == TokenEnum::Slash) {
        auto tok = curToken;
        next();
        x = std::make_unique<Arithmetic>(tok, std::move(x), unary());
    }
    return x;
}

std::unique_ptr<Expression> Parser::unary() {
    if (curToken->Type == TokenEnum::Minus) {
        auto tok = curToken;
        next();
        return std::make_unique<Unary>(tok, unary());
    }

    return factor();
}

std::unique_ptr<Expression> Parser::factor() {
    std::unique_ptr<Expression> x = nullptr;
    auto tok = curToken;
    switch (curToken->Type) {
    case TokenEnum::LParen:
        next();
        x = expression();
        match(TokenEnum::RParen);
        return std::move(x);
    case TokenEnum::Num:
    case TokenEnum::Real:
        x = std::make_unique<Constant>(curToken);
        next();
        return std::move(x);
    case TokenEnum::ID:
        next();
        if (curToken->Type != TokenEnum::LBracket) {
            return std::make_unique<Id>(tok);
        }
    default:
        error("syntax error");
        return x;
    }
}

std::unique_ptr<Expression> Parser::declaration() {
}

std::unique_ptr<Expression> Parser::identifier() {
}

void Parser::registerBasicTypes() {
    basicTypes.insert({"int", true});
    basicTypes.insert({"long", true});
    basicTypes.insert({"float", true});
    basicTypes.insert({"double", true});
    basicTypes.insert({"bool", true});
}
void Parser::registerKeywords() {
    keywords.insert({"if", true});
    keywords.insert({"else", true});
    keywords.insert({"for", true});
    keywords.insert({"func", true});
    keywords.insert({"package", true});
    keywords.insert({"struct", true});
}

bool Parser::isBasicType(std::string lexeme) {
    return basicTypes.count(lexeme) == 1;
}
bool Parser::isKeyword(std::string lexeme) {
    return keywords.count(lexeme) == 1;
}
