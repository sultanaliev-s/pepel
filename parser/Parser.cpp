#include "Parser.hpp"

void error(std::string message) {
    std::cout << message << std::endl;
    std::abort();
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
        error("syntax error, expected: " + TokenEnumToString(token) +
              " on line " + std::to_string(lexer->Line));
        return false;
    }
}

void Parser::error(std::string message) {
    std::cerr << message << std::endl;
    std::abort();
}

std::unique_ptr<Program> Parser::program() {
    auto prog = std::make_unique<Program>();
    prog->Statements = statements();

    return std::move(prog);
}

std::vector<std::unique_ptr<Statement>> Parser::statements() {
    std::vector<std::unique_ptr<Statement>> stmts(0);
    while (curToken->Type != TokenEnum::EndOfFile &&
           curToken->Type != TokenEnum::RBrace) {
        auto stmt = statement();
        if (curToken->Type == TokenEnum::Semicolon) {
            next();
        }
        stmts.push_back(std::move(stmt));
    }

    return std::move(stmts);
}

std::unique_ptr<Statement> Parser::statement() {
    switch (curToken->Type) {
    case TokenEnum::If:
        return ifStmt();
    default:
        return assign();
    }
}

std::unique_ptr<Statement> Parser::ifStmt() {
    next();

    auto cond = expression();
    if (cond == nullptr) {
        error("Expected a boolean expression");
        return nullptr;
    }

    std::unique_ptr<BlockStmt> ifBlock = blockStmt();
    std::unique_ptr<Statement> elseBlock;

    switch (curToken->Type) {
    case TokenEnum::Semicolon:
        next();
        return std::make_unique<If>(
            std::move(cond), std::move(ifBlock), nullptr);
    case TokenEnum::Else:
        next();
        if (curToken->Type == TokenEnum::If) {
            elseBlock = ifStmt();
        } else {
            elseBlock = blockStmt();
            match(TokenEnum::Semicolon);
        }
        return std::make_unique<If>(
            std::move(cond), std::move(ifBlock), std::move(elseBlock));
    default:
        error("Unexpected token after block in if statement");
        return nullptr;
    }
}

std::unique_ptr<BlockStmt> Parser::blockStmt() {
    match(TokenEnum::LBrace);

    auto block = std::make_unique<BlockStmt>();
    block->Statements = statements();

    match(TokenEnum::RBrace);
    return std::move(block);
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
                return std::make_unique<VariableDeclaration>(
                    word, id, std::move(expr));
            } else if (curToken->Type == TokenEnum::Semicolon) {
                return std::make_unique<VariableDeclaration>(word, id, nullptr);
            }
            error("Assing (var declaration) ended " + curToken->ToString());
            return nullptr;
        } else {
            auto id = std::static_pointer_cast<Word>(curToken);
            next();
            if (curToken->Type == TokenEnum::Assign) {
                next();
                auto expr = expression();
                match(TokenEnum::Semicolon);
                return std::make_unique<Set>(id, std::move(expr));
            }
            error("Assing (set) ended " + curToken->ToString());
            return nullptr;
        }
        error("Assign parser end " + curToken->ToString());
        return nullptr;
    }
    error("Assign parser did not match ID " + curToken->ToString());
    return nullptr;
    // switch (curToken->Type) {
    // case TokenEnum::ID:
    //     identifier();
    //     break;

    // default:
    //     break;
    // }
}

std::unique_ptr<Expression> Parser::expression() {
    auto x = join();
    while (curToken->Type == TokenEnum::Or) {
        auto tok = curToken;
        next();
        x = std::make_unique<Logical>(tok, std::move(x), join());
    }
    return std::move(x);
}

std::unique_ptr<Expression> Parser::join() {
    auto x = equality();
    while (curToken->Type == TokenEnum::And) {
        auto tok = curToken;
        next();
        x = std::make_unique<Logical>(tok, std::move(x), equality());
    }
    return std::move(x);
}

std::unique_ptr<Expression> Parser::equality() {
    auto x = relative();
    while (curToken->Type == TokenEnum::Equal ||
           curToken->Type == TokenEnum::NotEqual) {
        auto tok = curToken;
        next();
        x = std::make_unique<Logical>(tok, std::move(x), relative());
    }
    return std::move(x);
}

std::unique_ptr<Expression> Parser::relative() {
    auto x = arith();
    auto tok = curToken;
    switch (curToken->Type) {
    case TokenEnum::Greater:
    case TokenEnum::GreaterEqual:
    case TokenEnum::Less:
    case TokenEnum::LessEqual:
        next();
        return std::make_unique<Logical>(tok, std::move(x), arith());
    default:
        return std::move(x);
    }
}

std::unique_ptr<Expression> Parser::arith() {
    auto x = term();
    while (curToken->Type == TokenEnum::Plus ||
           curToken->Type == TokenEnum::Minus) {
        auto tok = curToken;
        next();
        x = std::make_unique<Arithmetic>(tok, std::move(x), term());
    }
    return std::move(x);
}

std::unique_ptr<Expression> Parser::term() {
    auto x = unary();
    while (curToken->Type == TokenEnum::Asterisk ||
           curToken->Type == TokenEnum::Slash) {
        auto tok = curToken;
        next();
        x = std::make_unique<Arithmetic>(tok, std::move(x), unary());
    }
    return std::move(x);
}

std::unique_ptr<Expression> Parser::unary() {
    if (curToken->Type == TokenEnum::Minus) {
        auto tok = curToken;
        next();
        return std::make_unique<Unary>(tok, unary());
        // } else if (curToken->Type == TokenEnum::Bang) {
        //     auto tok = curToken;
        //     next();
        //     return new Not(tok, unary());
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
    case TokenEnum::True:
        x = std::make_unique<Constant>(curToken);
        next();
        return std::move(x);
    case TokenEnum::False:
        x = std::make_unique<Constant>(curToken);
        next();
        return std::move(x);
    case TokenEnum::ID:
        next();
        if (curToken->Type != TokenEnum::LBracket) {
            return std::make_unique<Id>(tok);
        }
        // else {
        //     return offset(id);
        // }
    default:
        error("Unsupported factor " + curToken->ToString());
        return x;
    }
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
    keywords.insert({"true", true});
    keywords.insert({"false", true});
}

bool Parser::isBasicType(std::string lexeme) {
    return basicTypes.count(lexeme) == 1;
}
bool Parser::isKeyword(std::string lexeme) {
    return keywords.count(lexeme) == 1;
}
