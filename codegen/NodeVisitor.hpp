#pragma once
#include "llvm/IR/Value.h"

class Program;
class Statement;
class Expression;
class Id;
class Arithmetic;
class Operation;
class Set;
class VariableDeclaration;
class Constant;
class Unary;

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
