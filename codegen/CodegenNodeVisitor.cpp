#include "CodegenNodeVisitor.hpp"

CodegenNodeVisitor::CodegenNodeVisitor() {
    TheContext = std::make_unique<llvm::LLVMContext>();
    TheModule = std::make_unique<llvm::Module>("main module", *TheContext);

    // Create a new builder for the module.
    Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);

    auto i32 = Builder->getInt32Ty();
    auto prototype = llvm::FunctionType::get(i32, false);
    llvm::Function *main_fn = llvm::Function::Create(
        prototype, llvm::Function::ExternalLinkage, "main", TheModule.get());
    llvm::BasicBlock *body =
        llvm::BasicBlock::Create(*TheContext, "body", main_fn);
    Builder->SetInsertPoint(body);
}

llvm::Value *CodegenNodeVisitor::Visit(Program *node) {
    for (auto const &i : node->Statements) {
        i->Accept(this);
    }

    return nullptr;
}

llvm::Value *CodegenNodeVisitor::Visit(Statement *node) {
    return logError("Not implemented Statement");
}

llvm::Value *CodegenNodeVisitor::Visit(Expression *node) {
    return logError("Not implemented Expression");
}

llvm::Value *CodegenNodeVisitor::Visit(Id *node) {
    auto var = std::static_pointer_cast<Word>(node->Tok);
    if (NamedValues.count(var->Lexeme) == 0) {
        return logError("Unknown variable name" + var->Lexeme);
    }
    llvm::AllocaInst *alloca = NamedValues.find(var->Lexeme)->second;
    return Builder->CreateLoad(alloca->getAllocatedType(), alloca, var->Lexeme);
}

llvm::Value *CodegenNodeVisitor::Visit(Arithmetic *node) {
    llvm::Value *L = node->Left->Accept(this);
    llvm::Value *R = node->Right->Accept(this);
    if (!L || !R) {
        return nullptr;
    }

    switch (node->Op->Type) {
    case TokenEnum::Plus:
        return Builder->CreateAdd(L, R, "addtmp");
    case TokenEnum::Minus:
        return Builder->CreateSub(L, R, "subtmp");
    case TokenEnum::Asterisk:
        return Builder->CreateMul(L, R, "multmp");
    case TokenEnum::Slash:
        return Builder->CreateExactSDiv(L, R, "divtmp");
    default:
        return logError("invalid binary operator");
    }
}

llvm::Value *CodegenNodeVisitor::Visit(Operation *node) {
    return logError("Not implemented Operation");
}

llvm::Value *CodegenNodeVisitor::Visit(Set *node) {
    return logError("Not implemented Set");
}

llvm::Value *CodegenNodeVisitor::Visit(Unary *node) {
    return logError("Not implemented Unary");
}

llvm::Value *CodegenNodeVisitor::Visit(VariableDeclaration *node) {
    llvm::Type *variableType;
    if (node->Type->Lexeme == "int") {
        variableType = llvm::Type::getInt32Ty(*TheContext);
    } else if (node->Type->Lexeme == "float") {
        variableType = llvm::Type::getFloatTy(*TheContext);
    } else {
        return logError("unsupported type");
    }

    llvm::AllocaInst *variable =
        Builder->CreateAlloca(variableType, nullptr, node->Id->Lexeme);
    NamedValues.insert({node->Id->Lexeme, variable});

    if (node->Expr != nullptr) {
        llvm::Value *initialValue = node->Expr->Accept(this);
        Builder->CreateStore(initialValue, variable);
    }

    return variable;
}

llvm::Value *CodegenNodeVisitor::Visit(Constant *node) {
    if (node->Tok->Type == TokenEnum::Num) {
        auto num = std::static_pointer_cast<Number>(node->Tok);
        return llvm::ConstantInt::get(
            *TheContext, llvm::APInt(32, num->Value, true));
    } else {
        auto real = std::static_pointer_cast<RealNum>(node->Tok);
        return llvm::ConstantFP::get(*TheContext, llvm::APFloat(real->Value));
    }
}

llvm::Value *CodegenNodeVisitor::logError(std::string msg) {
    std::cerr << msg << std::endl;
    return nullptr;
}

int CodegenNodeVisitor::Compile(Program *prog) {
    prog->Accept(this);

    auto alloca = NamedValues.find("z")->second;
    auto ret = Builder->CreateLoad(alloca->getAllocatedType(), alloca, "z");
    Builder->CreateRet(ret);

    TheModule->print(llvm::errs(), nullptr);

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
        llvm::errs() << Error;
        return 1;
    }
    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();
    auto TheTargetMachine =
        Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    TheModule->setDataLayout(TheTargetMachine->createDataLayout());

    std::string Filename = "myprog";
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
    std::string cmd = "clang++ -no-pie  " + ObjectFilename + " -o " +
                      binaryFilename + " && rm " + ObjectFilename;
    std::system(cmd.c_str());

    llvm::outs() << "\nWrote " << binaryFilename << "\n";

    return 0;
}
