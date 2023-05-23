#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "../ast/AccessElement.hpp"
#include "../ast/Arithmetic.hpp"
#include "../ast/ArrayDeclaration.hpp"
#include "../ast/BlockStmt.hpp"
#include "../ast/BreakStmt.hpp"
#include "../ast/Call.hpp"
#include "../ast/Constant.hpp"
#include "../ast/ContinueStmt.hpp"
#include "../ast/Expression.hpp"
#include "../ast/ExpressionStmt.hpp"
#include "../ast/ForStmt.hpp"
#include "../ast/FuncStmt.hpp"
#include "../ast/Id.hpp"
#include "../ast/If.hpp"
#include "../ast/Logical.hpp"
#include "../ast/Operation.hpp"
#include "../ast/Program.hpp"
#include "../ast/ReturnStmt.hpp"
#include "../ast/Set.hpp"
#include "../ast/SetElement.hpp"
#include "../ast/Unary.hpp"
#include "../ast/VariableDeclaration.hpp"
#include "../lexer/Lexer.hpp"

class Parser {
   public:
   private:
    std::shared_ptr<Lexer> lexer;
    std::shared_ptr<Token> curToken;
    std::shared_ptr<Token> nextToken;
    std::map<std::string, bool> basicTypes;
    std::map<std::string, bool> keywords;

   public:
    Parser(std::shared_ptr<Lexer> _lexer);

    std::unique_ptr<Program> Parse();

   private:
    void next();
    bool match(TokenEnum token);
    bool isMatching(TokenEnum token);
    void error(std::string message);
    std::unique_ptr<Program> program();
    std::vector<std::unique_ptr<Statement>> statements();
    std::unique_ptr<Statement> statement();
    std::unique_ptr<Statement> ifStmt();
    std::unique_ptr<Statement> forStmt();
    std::unique_ptr<Statement> funcStmt();
    std::unique_ptr<Statement> returnStmt();
    std::unique_ptr<Statement> breakOrContinue();
    std::unique_ptr<Statement> expressionStmt();
    std::unique_ptr<BlockStmt> blockStmt();
    std::unique_ptr<Statement> assign();
    std::unique_ptr<Expression> expression();
    std::unique_ptr<Expression> call();
    std::unique_ptr<Expression> join();
    std::unique_ptr<Expression> equality();
    std::unique_ptr<Expression> relative();
    std::unique_ptr<Expression> arith();
    std::unique_ptr<Expression> factor();
    std::unique_ptr<Expression> term();
    std::unique_ptr<Expression> unary();
    std::unique_ptr<Expression> accessElement(std::shared_ptr<Token> id);
    std::unique_ptr<Set> set();
    std::unique_ptr<Statement> setElement();
    void registerBasicTypes();
    void registerKeywords();
    bool isBasicType(std::string lexeme);
    bool isKeyword(std::string lexeme);
};
