#pragma once
#include "BlockStmt.hpp"
#include "Expression.hpp"
#include "Set.hpp"

class ForStmt : public Statement {
   public:
    std::unique_ptr<Statement> Init;
    std::unique_ptr<Expression> Cond;
    std::unique_ptr<Set> Post;
    std::unique_ptr<BlockStmt> Block;

    ForStmt(std::unique_ptr<Statement> init, std::unique_ptr<Expression> cond,
        std::unique_ptr<Set> post, std::unique_ptr<BlockStmt> block)
        : Init(std::move(init)),
          Cond(std::move(cond)),
          Post(std::move(post)),
          Block(std::move(block)) {
    }

    llvm::Value* Accept(NodeVisitor* visitor) override {
        return visitor->Visit(this);
    }

    std::string ToString() override {
        std::string init = "";
        std::string cond = "";
        std::string post = "";
        if (Init != nullptr) {
            init = Init->ToString();
        }
        if (Cond != nullptr) {
            cond = Cond->ToString();
        }
        if (Post != nullptr) {
            post = Post->ToString();
        }
        return "For{" + init + "; " + cond + "; " + post + " " +
               Block->ToString() + "}";
    }
};
