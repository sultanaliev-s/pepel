#pragma once
#include <utility>

#include "BlockStmt.hpp"

class FuncStmt : public Statement {
   public:
    std::string Name;
    std::vector<std::pair<std::string, std::string>> Arguments;
    std::unique_ptr<std::string> ReturnType;
    std::unique_ptr<BlockStmt> Block;

    FuncStmt(std::string name,
        std::vector<std::pair<std::string, std::string>> arguments,
        std::unique_ptr<std::string> returnType,
        std::unique_ptr<BlockStmt> block)
        : Name(std::move(name)),
          Arguments(std::move(arguments)),
          ReturnType(std::move(returnType)),
          Block(std::move(block)) {
    }

    llvm::Value* Accept(NodeVisitor* visitor) override {
        return visitor->Visit(this);
    }

    std::string ToString() override {
        std::string args = "";
        for (auto& p : Arguments) {
            args += p.first + " " + p.second + ",";
        }
        std::string returnType = "";
        if (ReturnType != nullptr) {
            returnType = *ReturnType;
        }
        args = args.substr(0, args.length() - 1);
        return "Func " + Name + "(" + args + ")" + returnType +
               Block->ToString() + " " + Name + " ";
    }
};
