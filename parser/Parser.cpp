#include "Parser.hpp"

#include <iostream>

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

static std::unique_ptr<llvm::LLVMContext> TheContext;
static std::unique_ptr<llvm::IRBuilder<>> Builder;
static std::unique_ptr<llvm::Module> TheModule;
static std::map<std::string, llvm::AllocaInst*> NamedValues;

static void InitializeModule() {
    TheContext = std::make_unique<llvm::LLVMContext>();
    TheModule = std::make_unique<llvm::Module>("main module", *TheContext);

    // Create a new builder for the module.
    Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);

    auto i32 = Builder->getInt32Ty();
    auto prototype = llvm::FunctionType::get(i32, false);
    llvm::Function* main_fn = llvm::Function::Create(
        prototype, llvm::Function::ExternalLinkage, "main", TheModule.get());
    llvm::BasicBlock* body =
        llvm::BasicBlock::Create(*TheContext, "body", main_fn);
    Builder->SetInsertPoint(body);
}

static void compile() {
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
        return;
    }
    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();
    auto TheTargetMachine =
        Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    TheModule->setDataLayout(TheTargetMachine->createDataLayout());

    std::string Filename = "main";
    std::string ObjectFilename = Filename + ".o";
    std::error_code EC;
    llvm::raw_fd_ostream dest(ObjectFilename, EC, llvm::sys::fs::OF_None);

    if (EC) {
        llvm::errs() << "Could not open file: " << EC.message();
    }

    llvm::legacy::PassManager pass;
    auto FileType = llvm::CGFT_ObjectFile;

    if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
        llvm::errs() << "TheTargetMachine can't emit a file of this type";
    }

    pass.run(*TheModule);
    dest.flush();

    std::string binaryFilename = Filename + ".a";
    std::string cmd =
        "clang++ -no-pie  " + ObjectFilename + " -o " + binaryFilename;
    std::system(cmd.c_str());

    llvm::outs() << "\nWrote " << binaryFilename << "\n";
}

void error(std::string message) {
    std::cout << message << std::endl;
    std::abort();
}

class VariableDeclaration : public Statement {
   public:
    std::shared_ptr<Word> Type;
    std::shared_ptr<Word> Id;
    std::unique_ptr<Expression> Expr;

    VariableDeclaration(std::shared_ptr<Word> type, std::shared_ptr<Word> id,
        std::unique_ptr<Expression> expr)
        : Type(type), Id(id), Expr(std::move(expr)) {
    }

    llvm::Value* codegen() override {
        llvm::Type* variableType;
        if (Type->Lexeme == "int") {
            variableType = llvm::Type::getInt32Ty(*TheContext);
        } else if (Type->Lexeme == "float") {
            variableType = llvm::Type::getFloatTy(*TheContext);
        } else {
            error("unsupported type");
        }

        llvm::AllocaInst* variable =
            Builder->CreateAlloca(variableType, nullptr, Id->Lexeme);
        NamedValues.insert({Id->Lexeme, variable});

        if (Expr != nullptr) {
            llvm::Value* initialValue = Expr->codegen();
            Builder->CreateStore(initialValue, variable);
        }

        return variable;
    }

    std::string ToString() override {
        return "VarDecl{" + Type->ToString() + " " + Id->ToString() +
               Expr->ToString() + "}";
    }
};

class Constant : public Expression {
   public:
    std::shared_ptr<Token> Tok;

    Constant(std::shared_ptr<Token> token) : Tok(std::move(token)) {
    }

    llvm::Value* codegen() override {
        if (Tok->Type == TokenEnum::Num) {
            auto num = std::static_pointer_cast<Number>(Tok);
            return llvm::ConstantInt::get(
                *TheContext, llvm::APInt(32, num->Value, true));
        } else {
            auto real = std::static_pointer_cast<RealNum>(Tok);
            return llvm::ConstantFP::get(
                *TheContext, llvm::APFloat(real->Value));
        }
    }

    std::string ToString() override {
        return Tok->ToString();
    }
};

class Id : public Expression {
   public:
    std::shared_ptr<Token> Tok;

    Id(std::shared_ptr<Token> token) : Tok(std::move(token)) {
    }

    llvm::Value* codegen() override {
        auto var = std::static_pointer_cast<Word>(Tok);
        if (NamedValues.count(var->Lexeme) == 0) {
            error("Unknown variable name" + var->Lexeme);
        }
        llvm::AllocaInst* alloca = NamedValues.find(var->Lexeme)->second;
        return Builder->CreateLoad(alloca->getType(), alloca, var->Lexeme);
    }

    std::string ToString() override {
        return Tok->ToString();
    }
};

class Operation : public Expression {
   public:
    std::shared_ptr<Token> Op;

    Operation(std::shared_ptr<Token> op) : Op(op) {
    }

    llvm::Value* codegen() override {
        std::cout << "Gen op" << std::endl;
    }

    std::string ToString() override {
        return Op->ToString();
    }
};

class Arithmetic : public Operation {
   public:
    std::unique_ptr<Expression> Left;
    std::unique_ptr<Expression> Right;

    Arithmetic(std::shared_ptr<Token> op, std::unique_ptr<Expression> left,
        std::unique_ptr<Expression> right)
        : Operation(op), Left(std::move(left)), Right(std::move(right)) {
    }

    llvm::Value* codegen() override {
        llvm::Value* L = Left->codegen();
        llvm::Value* R = Right->codegen();
        if (!L || !R) {
            return nullptr;
        }

        switch (Op->Type) {
        case TokenEnum::Plus:
            return Builder->CreateAdd(L, R, "addtmp");
        case TokenEnum::Minus:
            return Builder->CreateSub(L, R, "subtmp");
        case TokenEnum::Asterisk:
            return Builder->CreateMul(L, R, "multmp");
        case TokenEnum::Slash:
            return Builder->CreateExactSDiv(L, R, "divtmp");
        default:
            error("invalid binary operator");
            return nullptr;
        }
    }

    std::string ToString() override {
        return "(" + Left->ToString() + Op->ToString() + Right->ToString() +
               ")";
    }
};

class Unary : public Operation {
   public:
    std::unique_ptr<Expression> Expr;

    Unary(std::shared_ptr<Token> op, std::unique_ptr<Expression> expr)
        : Operation(op), Expr(std::move(expr)) {
    }

    llvm::Value* codegen() override {
        std::cout << "Gen unary" << std::endl;
    }

    std::string ToString() override {
        return "(" + Op->ToString() + Expr->ToString() + ")";
    }
};

std::string Program::ToString() {
    std::string str = "Program{\n";
    for (size_t i = 0; i < Statements.size(); i++) {
        str += Statements[i]->ToString() + "\n";
    }
    str += "}";
    return str;
}

Parser::Parser(std::shared_ptr<Lexer> _lexer) : lexer(_lexer) {
    next();
    registerBasicTypes();
    registerKeywords();
    InitializeModule();
}

std::unique_ptr<Program> Parser::Parse() {
    return std::move(program());
}

void Parser::next() {
    curToken = lexer->Scan();
}

bool Parser::match(TokenEnum token) {
    if (curToken->Type == token) {
        next();
        return true;
    } else {
        error("syntax error");
    }
}

void Parser::error(std::string message) {
    std::cout << message;
    std::abort();
}

std::unique_ptr<Program> Parser::program() {
    auto prog = std::make_unique<Program>();
    while (curToken->Type != TokenEnum::EndOfFile) {
        auto stmt = statement();
        stmt->codegen();
        prog->Statements.push_back(std::move(stmt));
    }
    // auto ret = llvm::ConstantInt::get(Builder->getInt32Ty(), 0);
    auto alloca = NamedValues.find("z")->second;
    auto ret = Builder->CreateLoad(alloca->getType(), alloca, "z");
    Builder->CreateRet(ret);
    TheModule->print(llvm::errs(), nullptr);

    compile();
    return std::move(prog);
}

std::unique_ptr<Statement> Parser::statement() {
    auto stmt = assign();
    return std::move(stmt);
}

std::unique_ptr<Statement> Parser::assign() {
    if (curToken->Type == TokenEnum::ID) {
        auto word = std::static_pointer_cast<Word>(curToken);
        if (isBasicType(word->Lexeme)) {
            next();
            auto id = std::static_pointer_cast<Word>(curToken);
            next();
            if (curToken->Type == TokenEnum::Assign) {
                next();
                auto expr = expression();
                match(TokenEnum::Semicolon);
                return std::make_unique<VariableDeclaration>(
                    word, id, std::move(expr));
            } else if (curToken->Type == TokenEnum::Semicolon) {
                next();
                return std::make_unique<VariableDeclaration>(word, id, nullptr);
            }
        }
        std::cout << "assign func end" << std::endl;
    }

    switch (curToken->Type) {
    case TokenEnum::ID:
        identifier();
        break;

    default:
        break;
    }
}

std::unique_ptr<Expression> Parser::expression() {
    auto x = term();
    while (curToken->Type == TokenEnum::Plus ||
           curToken->Type == TokenEnum::Minus) {
        auto tok = curToken;
        next();
        x = std::make_unique<Arithmetic>(tok, std::move(x), term());
    }
    return x;
}

std::unique_ptr<Expression> Parser::term() {
    auto x = unary();
    while (curToken->Type == TokenEnum::Asterisk ||
           curToken->Type == TokenEnum::Slash) {
        auto tok = curToken;
        next();
        x = std::make_unique<Arithmetic>(tok, std::move(x), unary());
    }
    return x;
}

std::unique_ptr<Expression> Parser::unary() {
    if (curToken->Type == TokenEnum::Minus) {
        auto tok = curToken;
        next();
        return std::make_unique<Unary>(tok, unary());
        // } else if (curToken->Type == TokenEnum::Bang) {
        //     auto tok = curToken;
        //     next();
        //     return new Not(tok, unary());
    }

    return factor();
}

std::unique_ptr<Expression> Parser::factor() {
    std::unique_ptr<Expression> x = nullptr;
    auto tok = curToken;
    switch (curToken->Type) {
    case TokenEnum::LParen:
        next();
        x = expression();
        match(TokenEnum::RParen);
        return std::move(x);
    case TokenEnum::Num:
    case TokenEnum::Real:
        x = std::make_unique<Constant>(curToken);
        next();
        return std::move(x);
    // case TokenEnum::True:
    //     x = Constant._true;
    //     next();
    //     return x;
    // case TokenEnum::False:
    //     x = Constant._false;
    //     next();
    //     return x;
    case TokenEnum::ID:
        next();
        if (curToken->Type != TokenEnum::LBracket) {
            return std::make_unique<Id>(tok);
        }
        // else {
        //     return offset(id);
        // }
    default:
        error("syntax error");
        return x;
    }
}

std::unique_ptr<Expression> Parser::declaration() {
}

std::unique_ptr<Expression> Parser::identifier() {
}

void Parser::registerBasicTypes() {
    basicTypes.insert({"int", true});
    basicTypes.insert({"long", true});
    basicTypes.insert({"float", true});
    basicTypes.insert({"double", true});
    basicTypes.insert({"bool", true});
}
void Parser::registerKeywords() {
    keywords.insert({"if", true});
    keywords.insert({"else", true});
    keywords.insert({"for", true});
    keywords.insert({"func", true});
    keywords.insert({"package", true});
    keywords.insert({"struct", true});
}

bool Parser::isBasicType(std::string lexeme) {
    return basicTypes.count(lexeme) == 1;
}
bool Parser::isKeyword(std::string lexeme) {
    return keywords.count(lexeme) == 1;
}
