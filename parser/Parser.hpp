#pragma once
#include <map>
#include <memory>
#include <string>

#include "../lexer/Lexer.hpp"
#include "Expression.hpp"
#include "Program.hpp"
#include "Set.hpp"

class Parser {
   public:
   private:
    std::shared_ptr<Lexer> lexer;
    std::shared_ptr<Token> curToken;
    std::map<std::string, bool> basicTypes;
    std::map<std::string, bool> keywords;

   public:
    Parser(std::shared_ptr<Lexer> _lexer);

    std::unique_ptr<Program> Parse();

   private:
    void next();
    bool match(TokenEnum token);
    void error(std::string message);
    std::unique_ptr<Program> program();
    std::unique_ptr<Statement> statement();
    std::unique_ptr<Statement> assign();
    std::unique_ptr<Expression> expression();
    std::unique_ptr<Expression> identifier();
    std::unique_ptr<Expression> declaration();
    std::unique_ptr<Expression> factor();
    std::unique_ptr<Expression> term();
    std::unique_ptr<Expression> unary();
    void registerBasicTypes();
    void registerKeywords();
    bool isBasicType(std::string lexeme);
    bool isKeyword(std::string lexeme);
};
