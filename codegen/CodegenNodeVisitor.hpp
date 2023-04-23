#pragma once
#include <iostream>
#include <map>
#include <stack>

#include "../ast/Arithmetic.hpp"
#include "../ast/BlockStmt.hpp"
#include "../ast/BreakStmt.hpp"
#include "../ast/Constant.hpp"
#include "../ast/ContinueStmt.hpp"
#include "../ast/ForStmt.hpp"
#include "../ast/FuncStmt.hpp"
#include "../ast/Id.hpp"
#include "../ast/If.hpp"
#include "../ast/Logical.hpp"
#include "../ast/Node.hpp"
#include "../ast/Operation.hpp"
#include "../ast/Program.hpp"
#include "../ast/ReturnStmt.hpp"
#include "../ast/Set.hpp"
#include "../ast/Unary.hpp"
#include "../ast/VariableDeclaration.hpp"
#include "../lexer/RealNum.hpp"
#include "NodeVisitor.hpp"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

class CodegenNodeVisitor : public NodeVisitor {
    std::unique_ptr<llvm::LLVMContext> TheContext;
    std::unique_ptr<llvm::IRBuilder<>> Builder;
    std::unique_ptr<llvm::Module> TheModule;
    std::map<std::string, llvm::AllocaInst *> NamedValues;
    std::stack<llvm::BasicBlock *> loopStack;

   public:
    CodegenNodeVisitor();
    int Compile(Program *prog, std::string fileName);

    llvm::Value *Visit(Program *node) override;
    llvm::Value *Visit(Statement *node) override;
    llvm::Value *Visit(Expression *node) override;
    llvm::Value *Visit(Id *node) override;
    llvm::Value *Visit(Logical *node) override;
    llvm::Value *Visit(Arithmetic *node) override;
    llvm::Value *Visit(Operation *node) override;
    llvm::Value *Visit(Set *node) override;
    llvm::Value *Visit(Unary *node) override;
    llvm::Value *Visit(VariableDeclaration *node) override;
    llvm::Value *Visit(Constant *node) override;
    llvm::Value *Visit(If *node) override;
    llvm::Value *Visit(BlockStmt *node) override;
    llvm::Value *Visit(ForStmt *node) override;
    llvm::Value *Visit(BreakStmt *node) override;
    llvm::Value *Visit(ContinueStmt *node) override;
    llvm::Value *Visit(FuncStmt *node) override;
    llvm::Value *Visit(ReturnStmt *node) override;

   private:
    llvm::Value *logError(std::string);
    llvm::Value *intArithmetic(llvm::Value *l, llvm::Value *r, TokenEnum op);
    llvm::Value *floatArithmetic(llvm::Value *l, llvm::Value *r, TokenEnum op);
    llvm::Value *boolLogic(llvm::Value *l, llvm::Value *r, TokenEnum op);
    llvm::Value *intLogic(llvm::Value *l, llvm::Value *r, TokenEnum op);
    llvm::Value *floatLogic(llvm::Value *l, llvm::Value *r, TokenEnum op);
    llvm::Type *getBasicType(std::string type);
    llvm::AllocaInst *createEntryBlockAlloca(
        llvm::Function *func, llvm::Type *type, const std::string &varName);
};
