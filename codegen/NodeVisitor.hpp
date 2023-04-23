#pragma once
#include "llvm/IR/Value.h"

class Arithmetic;
class BlockStmt;
class BreakStmt;
class Constant;
class ContinueStmt;
class Expression;
class ForStmt;
class FuncStmt;
class Id;
class If;
class Logical;
class Operation;
class Program;
class ReturnStmt;
class Set;
class Statement;
class Unary;
class VariableDeclaration;

class NodeVisitor {
   public:
    virtual ~NodeVisitor() = default;

    virtual llvm::Value *Visit(Program *node) = 0;
    virtual llvm::Value *Visit(Statement *node) = 0;
    virtual llvm::Value *Visit(Expression *node) = 0;
    virtual llvm::Value *Visit(Id *node) = 0;
    virtual llvm::Value *Visit(Logical *node) = 0;
    virtual llvm::Value *Visit(Arithmetic *node) = 0;
    virtual llvm::Value *Visit(Operation *node) = 0;
    virtual llvm::Value *Visit(Set *node) = 0;
    virtual llvm::Value *Visit(Unary *node) = 0;
    virtual llvm::Value *Visit(VariableDeclaration *node) = 0;
    virtual llvm::Value *Visit(Constant *node) = 0;
    virtual llvm::Value *Visit(If *node) = 0;
    virtual llvm::Value *Visit(BlockStmt *node) = 0;
    virtual llvm::Value *Visit(ForStmt *node) = 0;
    virtual llvm::Value *Visit(BreakStmt *node) = 0;
    virtual llvm::Value *Visit(ContinueStmt *node) = 0;
    virtual llvm::Value *Visit(FuncStmt *node) = 0;
    virtual llvm::Value *Visit(ReturnStmt *node) = 0;
};
