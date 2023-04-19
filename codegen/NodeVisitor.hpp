#pragma once

#include "../ast/Arithmetic.hpp"
#include "../ast/Constant.hpp"
#include "../ast/Id.hpp"
#include "../ast/Operation.hpp"
#include "../ast/Program.hpp"
#include "../ast/Set.hpp"
#include "../ast/Unary.hpp"
#include "../ast/VariableDeclaration.hpp"
#include "llvm/IR/Value.h"

class NodeVisitor {
   public:
    virtual ~NodeVisitor() = default;

    virtual llvm::Value *Visit(Program *node) = 0;
    virtual llvm::Value *Visit(Statement *node) = 0;
    virtual llvm::Value *Visit(Expression *node) = 0;
    virtual llvm::Value *Visit(Id *node) = 0;
    virtual llvm::Value *Visit(Arithmetic *node) = 0;
    virtual llvm::Value *Visit(Operation *node) = 0;
    virtual llvm::Value *Visit(Set *node) = 0;
    virtual llvm::Value *Visit(Unary *node) = 0;
    virtual llvm::Value *Visit(VariableDeclaration *node) = 0;
    virtual llvm::Value *Visit(Constant *node) = 0;
};
