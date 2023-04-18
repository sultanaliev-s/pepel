#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
static llvm::LLVMContext TheContext;
static llvm::IRBuilder<> Builder(TheContext);
static std::unique_ptr<llvm::Module> TheModule;
static std::map<std::string, llvm::Value*> NamedValues;

int main() {
    TheModule = std::make_unique<llvm::Module>("module", TheContext);

    auto i32 = Builder.getInt32Ty();
    auto prototype = llvm::FunctionType::get(i32, false);
    llvm::Function* main_fn = llvm::Function::Create(
        prototype, llvm::Function::ExternalLinkage, "main", TheModule.get());
    llvm::BasicBlock* body =
        llvm::BasicBlock::Create(TheContext, "body", main_fn);
    Builder.SetInsertPoint(body);

    llvm::Type* variableType = llvm::Type::getInt32Ty(TheContext);
    std::cout << 1 << std::endl;

    llvm::AllocaInst* variable =
        Builder.CreateAlloca(variableType, nullptr, "x");
    std::cout << 2 << std::endl;

    auto value = llvm::ConstantInt::get(TheContext, llvm::APInt(32, 42, true));
    std::cout << 3 << std::endl;

    llvm::Value* initialValue = value;
    Builder.CreateStore(initialValue, variable);
    std::cout << 4 << std::endl;

    // use libc's printf function
    auto i8p = Builder.getInt8PtrTy();
    auto printf_prototype = llvm::FunctionType::get(i8p, true);
    auto printf_fn = llvm::Function::Create(printf_prototype,
        llvm::Function::ExternalLinkage, "printf", TheModule.get());

    // call printf with our string
    auto format_str = Builder.CreateGlobalStringPtr("%d\n");
    Builder.CreateCall(printf_fn, {format_str, variable});

    // return 0 from main
    auto ret = llvm::ConstantInt::get(i32, 0);
    Builder.CreateRet(Builder.CreateLoad(variableType, variable, "x"));

    TheModule->print(llvm::errs(), nullptr);

    return 0;
}
