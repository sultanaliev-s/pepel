#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <system_error>

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

    llvm::AllocaInst* variable =
        Builder.CreateAlloca(variableType, nullptr, "x");

    auto value = llvm::ConstantInt::get(TheContext, llvm::APInt(32, 42, true));

    llvm::Value* initialValue = value;
    Builder.CreateStore(initialValue, variable);

    // use libc's printf function
    auto i8p = Builder.getInt8PtrTy();
    auto printf_prototype = llvm::FunctionType::get(i8p, true);
    auto printf_fn = llvm::Function::Create(printf_prototype,
        llvm::Function::ExternalLinkage, "printf", TheModule.get());

    // call printf with our string
    auto format_str = Builder.CreateGlobalStringPtr("%d is the answer\n");
    Builder.CreateCall(printf_fn,
        {format_str, Builder.CreateLoad(variableType, variable, "x")});

    // return 0 from main
    auto ret = llvm::ConstantInt::get(i32, 0);
    Builder.CreateRet(ret);

    TheModule->print(llvm::outs(), nullptr);

    // compilation-- -- -- -- -- -- --compilation-- -- -- -- -- -- -compilation
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
    auto TargetTriple = llvm::sys::getDefaultTargetTriple();
    std::string Error;
    auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

    // Print an error and exit if we couldn't find the requested target.
    // This generally occurs if we've forgotten to initialise the
    // TargetRegistry or we have a bogus target triple.
    if (!Target) {
        std::cout << "No target" << std::endl;
        return 1;
    }

    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();
    auto TheTargetMachine =
        Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    TheModule->setDataLayout(TheTargetMachine->createDataLayout());

    std::string Filename = "output";
    std::string ObjectFilename = Filename + ".o";
    std::error_code EC;
    llvm::raw_fd_ostream dest(ObjectFilename, EC, llvm::sys::fs::OF_None);

    if (EC) {
        llvm::errs() << "Could not open file: " << EC.message();
        return 1;
    }

    llvm::legacy::PassManager pass;
    auto FileType = llvm::CGFT_ObjectFile;

    if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
        llvm::errs() << "TheTargetMachine can't emit a file of this type";
        return 1;
    }

    pass.run(*TheModule);
    dest.flush();

    std::string binaryFilename = Filename + ".a";
    std::string cmd =
        "clang++ -no-pie  " + ObjectFilename + " -o " + binaryFilename;
    std::system(cmd.c_str());
    llvm::outs() << "\nWrote " << binaryFilename << "\n";

    return 0;
}
