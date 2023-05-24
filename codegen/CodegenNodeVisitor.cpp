#include "CodegenNodeVisitor.hpp"

CodegenNodeVisitor::CodegenNodeVisitor() {
    TheContext = std::make_unique<llvm::LLVMContext>();
    TheModule = std::make_unique<llvm::Module>("main module", *TheContext);

    // Create a new builder for the module.
    Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);

    scope = std::make_unique<Scope>();
    createPrintFuncs();
    createScanFuncs();
    createExitFuncs();
    createStrings();
}

void CodegenNodeVisitor::createPrintFuncs() {
    // use libc's printf function
    auto i8p = Builder->getInt8PtrTy();
    auto printf_prototype = llvm::FunctionType::get(i8p, true);
    auto printf_fn = llvm::Function::Create(printf_prototype,
        llvm::Function::ExternalLinkage, "printf", TheModule.get());
    {
        // Declare function Print(int)
        std::vector<llvm::Type *> argTypes = {Builder->getInt32Ty()};
        llvm::FunctionType *printIntFuncType =
            llvm::FunctionType::get(Builder->getVoidTy(), argTypes, false);

        std::string printFuncName = mangleFunction("Print", argTypes);
        llvm::Function *printIntFunc = llvm::Function::Create(printIntFuncType,
            llvm::Function::ExternalLinkage, printFuncName, TheModule.get());
        printIntFunc->setDoesNotThrow();

        // Print(int) implementation
        llvm::BasicBlock *printIntBB =
            llvm::BasicBlock::Create(*TheContext, "body", printIntFunc);
        Builder->SetInsertPoint(printIntBB);
        llvm::Function::arg_iterator args = printIntFunc->arg_begin();
        llvm::Value *arg_x = args++;
        llvm::Value *formatStr =
            Builder->CreateGlobalStringPtr("%d", "formatStrInt");
        std::vector<llvm::Value *> printfArgs = {formatStr, arg_x};
        Builder->CreateCall(printf_fn, printfArgs);
        Builder->CreateRetVoid();
    }
    {
        // Declare function Println(int)
        std::vector<llvm::Type *> argTypes = {Builder->getInt32Ty()};
        llvm::FunctionType *printIntFuncType =
            llvm::FunctionType::get(Builder->getVoidTy(), argTypes, false);
        std::string printlnFuncName = mangleFunction("Println", argTypes);
        llvm::Function *printIntFunc = llvm::Function::Create(printIntFuncType,
            llvm::Function::ExternalLinkage, printlnFuncName, TheModule.get());
        printIntFunc->setDoesNotThrow();

        // Println(int) implementation
        llvm::BasicBlock *printIntBB =
            llvm::BasicBlock::Create(*TheContext, "body", printIntFunc);
        Builder->SetInsertPoint(printIntBB);
        llvm::Function::arg_iterator args = printIntFunc->arg_begin();
        llvm::Value *arg_x = args++;
        llvm::Value *formatStr =
            Builder->CreateGlobalStringPtr("%d\n", "formatStrIntln");
        std::vector<llvm::Value *> printfArgs = {formatStr, arg_x};
        Builder->CreateCall(printf_fn, printfArgs);
        Builder->CreateRetVoid();
    }
    {
        // Declare function Print(float)
        std::vector<llvm::Type *> argTypes = {Builder->getFloatTy()};
        llvm::FunctionType *printFloatFuncType =
            llvm::FunctionType::get(Builder->getVoidTy(), argTypes, false);

        std::string funcName = mangleFunction("Print", argTypes);
        llvm::Function *printFloatFunc =
            llvm::Function::Create(printFloatFuncType,
                llvm::Function::ExternalLinkage, funcName, TheModule.get());
        printFloatFunc->setDoesNotThrow();

        // Print(float) implementation
        llvm::BasicBlock *printFloatBB =
            llvm::BasicBlock::Create(*TheContext, "body", printFloatFunc);
        Builder->SetInsertPoint(printFloatBB);
        llvm::Function::arg_iterator args = printFloatFunc->arg_begin();
        llvm::Value *arg_y = args++;
        llvm::Value *casted_value =
            Builder->CreateFPExt(arg_y, llvm::Type::getDoubleTy(*TheContext));
        llvm::Value *formatStr =
            Builder->CreateGlobalStringPtr("%f", "formatStrFloat");
        std::vector<llvm::Value *> printfArgs = {formatStr, casted_value};
        Builder->CreateCall(printf_fn, printfArgs);
        Builder->CreateRetVoid();
    }
    {
        // Declare function Println(float)
        std::vector<llvm::Type *> argTypes = {Builder->getFloatTy()};
        llvm::FunctionType *printFloatFuncType =
            llvm::FunctionType::get(Builder->getVoidTy(), argTypes, false);

        std::string funcName = mangleFunction("Println", argTypes);
        llvm::Function *printFloatFunc =
            llvm::Function::Create(printFloatFuncType,
                llvm::Function::ExternalLinkage, funcName, TheModule.get());
        printFloatFunc->setDoesNotThrow();

        // Println(float) implementation
        llvm::BasicBlock *printFloatBB =
            llvm::BasicBlock::Create(*TheContext, "body", printFloatFunc);
        Builder->SetInsertPoint(printFloatBB);
        llvm::Function::arg_iterator args = printFloatFunc->arg_begin();
        llvm::Value *arg_y = args++;
        llvm::Value *casted_value =
            Builder->CreateFPExt(arg_y, llvm::Type::getDoubleTy(*TheContext));
        llvm::Value *formatStr =
            Builder->CreateGlobalStringPtr("%f\n", "formatStrFloatln");
        std::vector<llvm::Value *> printfArgs = {formatStr, casted_value};
        Builder->CreateCall(printf_fn, printfArgs);
        Builder->CreateRetVoid();
    }
    {  // Declare function Print(string)
        std::vector<llvm::Type *> argTypes = {Builder->getInt8PtrTy()};
        llvm::FunctionType *printStringFuncType =
            llvm::FunctionType::get(Builder->getVoidTy(), argTypes, false);

        std::string funcName = mangleFunction("Print", argTypes);
        llvm::Function *printStringFunc =
            llvm::Function::Create(printStringFuncType,
                llvm::Function::ExternalLinkage, funcName, TheModule.get());
        printStringFunc->setDoesNotThrow();

        // Print(string) implementation
        llvm::BasicBlock *printIntBB =
            llvm::BasicBlock::Create(*TheContext, "body", printStringFunc);
        Builder->SetInsertPoint(printIntBB);
        llvm::Function::arg_iterator args = printStringFunc->arg_begin();
        llvm::Value *arg_x = args++;
        llvm::Value *formatStr =
            Builder->CreateGlobalStringPtr("%s", "formatStrString");
        std::vector<llvm::Value *> printfArgs = {formatStr, arg_x};
        Builder->CreateCall(printf_fn, printfArgs);
        Builder->CreateRetVoid();
    }
    {  // Declare function Println(string)
        std::vector<llvm::Type *> argTypes = {Builder->getInt8PtrTy()};
        llvm::FunctionType *printStringFuncType =
            llvm::FunctionType::get(Builder->getVoidTy(), argTypes, false);

        std::string funcName = mangleFunction("Println", argTypes);
        llvm::Function *printStringFunc =
            llvm::Function::Create(printStringFuncType,
                llvm::Function::ExternalLinkage, funcName, TheModule.get());
        printStringFunc->setDoesNotThrow();

        // Println(string) implementation
        llvm::BasicBlock *printIntBB =
            llvm::BasicBlock::Create(*TheContext, "body", printStringFunc);
        Builder->SetInsertPoint(printIntBB);
        llvm::Function::arg_iterator args = printStringFunc->arg_begin();
        llvm::Value *arg_x = args++;
        llvm::Value *formatStr =
            Builder->CreateGlobalStringPtr("%s\n", "formatStrStringln");
        std::vector<llvm::Value *> printfArgs = {formatStr, arg_x};
        Builder->CreateCall(printf_fn, printfArgs);
        Builder->CreateRetVoid();
    }
}

void CodegenNodeVisitor::createScanFuncs() {
    auto intType = Builder->getInt32Ty();
    auto scanIntName = "ScanInt__";
    auto scanInt_prototype = llvm::FunctionType::get(intType, false);
    auto scanInt_fn = llvm::Function::Create(scanInt_prototype,
        llvm::Function::ExternalLinkage, scanIntName, TheModule.get());

    auto floatType = Builder->getFloatTy();
    auto scanFloatName = "ScanFloat__";
    auto scanFloat_prototype = llvm::FunctionType::get(floatType, false);
    auto scanFloat_fn = llvm::Function::Create(scanFloat_prototype,
        llvm::Function::ExternalLinkage, scanFloatName, TheModule.get());
}

void CodegenNodeVisitor::createExitFuncs() {
    auto i8p = Builder->getInt8PtrTy();
    auto intType = Builder->getInt32Ty();
    auto exitWithError_prototype =
        llvm::FunctionType::get(Builder->getVoidTy(), {i8p, intType}, false);
    auto exitWithError_fn = llvm::Function::Create(exitWithError_prototype,
        llvm::Function::ExternalLinkage, "exitWithError", TheModule.get());
}

void CodegenNodeVisitor::createStrings() {
    globalStrings.insert(
        {"empty", Builder->CreateGlobalStringPtr("", "emptyString")});
    globalStrings.insert(
        {"outOfBounds", Builder->CreateGlobalStringPtr(
                            "Index out of bounds: ", "outOfBounds")});
    globalStrings.insert({"negativeIndex",
        Builder->CreateGlobalStringPtr("Negative index: ", "negativeIndex")});
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
    auto variableDetails = scope->Get(var->Lexeme);
    if (variableDetails == nullptr) {
        return logError("Unknown variable name: " + var->Lexeme);
    }
    llvm::AllocaInst *variable = variableDetails->Alloca;

    return Builder->CreateLoad(
        variable->getAllocatedType(), variable, var->Lexeme);
}

llvm::Value *CodegenNodeVisitor::Visit(Logical *node) {
    llvm::Value *L = node->Left->Accept(this);
    llvm::Value *R = node->Right->Accept(this);
    if (!L || !R) {
        return nullptr;
    }
    if (L->getType() != R->getType()) {
        return logError("Types don't match for " + node->Left->ToString() +
                        " " + node->Right->ToString());
    }

    auto type = L->getType();

    if (type == llvm::Type::getInt1Ty(*TheContext)) {
        return boolLogic(L, R, node->Op->Type);
    } else if (type == llvm::Type::getInt32Ty(*TheContext)) {
        return intLogic(L, R, node->Op->Type);
    } else if (type == llvm::Type::getFloatTy(*TheContext)) {
        return floatLogic(L, R, node->Op->Type);
    }

    return logError("Unsupported type for logical");
}

llvm::Value *CodegenNodeVisitor::boolLogic(
    llvm::Value *l, llvm::Value *r, TokenEnum op) {
    switch (op) {
    case TokenEnum::And:
        return Builder->CreateAnd(l, r, "andtmp");
    case TokenEnum::Or:
        return Builder->CreateOr(l, r, "ortmp");
    case TokenEnum::Equal:
        return Builder->CreateICmpEQ(l, r, "eqtmp");
    case TokenEnum::NotEqual:
        return Builder->CreateICmpNE(l, r, "neqtmp");
    default:
        return logError("Invalid logical operator for boolean");
    }
}

llvm::Value *CodegenNodeVisitor::intLogic(
    llvm::Value *l, llvm::Value *r, TokenEnum op) {
    switch (op) {
    case TokenEnum::Greater:
        return Builder->CreateICmpSGT(l, r, "sgttmp");
    case TokenEnum::GreaterEqual:
        return Builder->CreateICmpSGE(l, r, "sgetmp");
    case TokenEnum::Less:
        return Builder->CreateICmpSLT(l, r, "slttmp");
    case TokenEnum::LessEqual:
        return Builder->CreateICmpSLE(l, r, "sletmp");
    case TokenEnum::Equal:
        return Builder->CreateICmpEQ(l, r, "eqtmp");
    case TokenEnum::NotEqual:
        return Builder->CreateICmpNE(l, r, "neqtmp");
    default:
        return logError("Invalid logical operator for int");
    }
}

llvm::Value *CodegenNodeVisitor::floatLogic(
    llvm::Value *l, llvm::Value *r, TokenEnum op) {
    switch (op) {
    case TokenEnum::Greater:
        return Builder->CreateFCmpOGT(l, r, "sgttmp");
    case TokenEnum::GreaterEqual:
        return Builder->CreateFCmpOGE(l, r, "sgetmp");
    case TokenEnum::Less:
        return Builder->CreateFCmpOLT(l, r, "slttmp");
    case TokenEnum::LessEqual:
        return Builder->CreateFCmpOLE(l, r, "sletmp");
    case TokenEnum::Equal:
        return Builder->CreateFCmpOEQ(l, r, "eqtmp");
    case TokenEnum::NotEqual:
        return Builder->CreateFCmpONE(l, r, "neqtmp");
    default:
        return logError("Invalid logical operator for int");
    }
}

llvm::Value *CodegenNodeVisitor::Visit(Arithmetic *node) {
    llvm::Value *L = node->Left->Accept(this);
    llvm::Value *R = node->Right->Accept(this);
    if (!L || !R) {
        return nullptr;
    }
    if (L->getType() != R->getType()) {
        return logError("Types don't match");
    }

    auto type = L->getType();

    if (type == llvm::Type::getInt32Ty(*TheContext)) {
        return intArithmetic(L, R, node->Op->Type);
    } else if (type == llvm::Type::getFloatTy(*TheContext)) {
        return floatArithmetic(L, R, node->Op->Type);
    }

    return logError("Unsupported type for arithmetics");
}

llvm::Value *CodegenNodeVisitor::intArithmetic(
    llvm::Value *l, llvm::Value *r, TokenEnum op) {
    switch (op) {
    case TokenEnum::Plus:
        return Builder->CreateAdd(l, r, "addtmp");
    case TokenEnum::Minus:
        return Builder->CreateSub(l, r, "subtmp");
    case TokenEnum::Asterisk:
        return Builder->CreateMul(l, r, "multmp");
    case TokenEnum::Slash:
        return Builder->CreateExactSDiv(l, r, "divtmp");
    default:
        return logError("Invalid binary operator");
    }
}

llvm::Value *CodegenNodeVisitor::floatArithmetic(
    llvm::Value *l, llvm::Value *r, TokenEnum op) {
    switch (op) {
    case TokenEnum::Plus:
        return Builder->CreateFAdd(l, r, "addtmp");
    case TokenEnum::Minus:
        return Builder->CreateFSub(l, r, "subtmp");
    case TokenEnum::Asterisk:
        return Builder->CreateFMul(l, r, "multmp");
    case TokenEnum::Slash:
        return Builder->CreateFDiv(l, r, "divtmp");
    default:
        return logError("Invalid binary operator");
    }
}

llvm::Value *CodegenNodeVisitor::Visit(Operation *node) {
    return logError("Not implemented Operation");
}

llvm::Value *CodegenNodeVisitor::Visit(Set *node) {
    auto word = std::static_pointer_cast<Word>(node->Id);

    auto variableDetails = scope->Get(word->Lexeme);
    llvm::AllocaInst *variable = variableDetails->Alloca;
    if (variable == nullptr) {
        return logError("Unknown variable name: " + word->Lexeme);
    }

    if (node->Expr != nullptr) {
        llvm::Value *initialValue = node->Expr->Accept(this);
        Builder->CreateStore(initialValue, variable);
    }

    return variable;
}

llvm::Value *CodegenNodeVisitor::Visit(Unary *node) {
    auto expr = node->Expr->Accept(this);
    if (expr->getType() == llvm::Type::getFloatTy(*TheContext)) {
        return Builder->CreateFNeg(expr);
    }
    return Builder->CreateNeg(expr);
}

llvm::Value *CodegenNodeVisitor::Visit(VariableDeclaration *node) {
    llvm::Type *variableType;
    if (node->Type->Lexeme == "int") {
        variableType = llvm::Type::getInt32Ty(*TheContext);
    } else if (node->Type->Lexeme == "long") {
        variableType = llvm::Type::getInt64Ty(*TheContext);
    } else if (node->Type->Lexeme == "float") {
        variableType = llvm::Type::getFloatTy(*TheContext);
    } else if (node->Type->Lexeme == "double") {
        variableType = llvm::Type::getDoubleTy(*TheContext);
    } else if (node->Type->Lexeme == "bool") {
        variableType = llvm::Type::getInt1Ty(*TheContext);
    } else if (node->Type->Lexeme == "string") {
        variableType = llvm::Type::getInt8PtrTy(*TheContext);
    } else {
        return logError("Unsupported type for declaration");
    }

    llvm::AllocaInst *variable =
        createEntryBlockAlloca(Builder->GetInsertBlock()->getParent(),
            variableType, nullptr, node->Id->Lexeme);
    scope->Put(node->Id->Lexeme, variable);

    if (node->Expr != nullptr) {
        llvm::Value *initialValue = node->Expr->Accept(this);
        Builder->CreateStore(initialValue, variable);
    } else if (node->Type->Lexeme == "string") {
        Builder->CreateStore(globalStrings.find("empty")->second, variable);
    } else {
        Builder->CreateStore(
            llvm::Constant::getNullValue(variableType), variable);
    }

    return variable;
}

llvm::Value *CodegenNodeVisitor::Visit(ArrayDeclaration *node) {
    llvm::Type *variableType;
    if (node->Type->Lexeme == "int") {
        variableType = llvm::Type::getInt32Ty(*TheContext);
    } else if (node->Type->Lexeme == "long") {
        variableType = llvm::Type::getInt64Ty(*TheContext);
    } else if (node->Type->Lexeme == "float") {
        variableType = llvm::Type::getFloatTy(*TheContext);
    } else if (node->Type->Lexeme == "double") {
        variableType = llvm::Type::getDoubleTy(*TheContext);
    } else if (node->Type->Lexeme == "bool") {
        variableType = llvm::Type::getInt1Ty(*TheContext);
    } else if (node->Type->Lexeme == "string") {
        variableType = llvm::Type::getInt8PtrTy(*TheContext);
    } else {
        return logError("Unsupported type for declaration");
    }

    auto intType = llvm::Type::getInt32Ty(*TheContext);

    llvm::AllocaInst *variable = createEntryBlockAlloca(
        Builder->GetInsertBlock()->getParent(), variableType,
        llvm::ConstantInt::getSigned(intType, node->Size), node->Id->Lexeme);
    scope->Put(node->Id->Lexeme, variable, node->Size);

    if (node->Expr != nullptr) {
        logError("Arrays don't support initialization yet");
        return nullptr;
    } else {
        llvm::Type *variablePtrType =
            variable->getType()->getPointerElementType();
        for (int i = 0, size = node->Size; i < size; i++) {
            llvm::Value *arrayElementPtr =
                Builder->CreateInBoundsGEP(variablePtrType, variable,
                    llvm::ConstantInt::getSigned(intType, i),
                    "arrayElem" + std::to_string(i) + "_");
            if (node->Type->Lexeme == "string") {
                auto store = Builder->CreateStore(
                    globalStrings.find("empty")->second, arrayElementPtr);
            } else {
                auto store = Builder->CreateStore(
                    llvm::Constant::getNullValue(variableType),
                    arrayElementPtr);
            }
        }
    }

    return variable;
}

llvm::Value *CodegenNodeVisitor::Visit(SetElement *node) {
    auto variableDetails = scope->Get(node->Id->Lexeme);
    llvm::AllocaInst *variable = variableDetails->Alloca;
    if (variable == nullptr) {
        return logError("Unknown variable name: " + node->Id->Lexeme);
    }
    llvm::Value *index = node->Index->Accept(this);
    if (index->getType() != llvm::Type::getInt32Ty(*TheContext)) {
        return logError("Index should be an integer value");
    }

    createInBoundsCheck(variableDetails, index);

    llvm::Value *arrayElementPtr = Builder->CreateInBoundsGEP(
        variable->getType()->getPointerElementType(), variable, index);

    llvm::Value *newValue = node->Expr->Accept(this);

    return Builder->CreateStore(newValue, arrayElementPtr);
}

llvm::Value *CodegenNodeVisitor::Visit(AccessElement *node) {
    auto variableDetails = scope->Get(node->Id);
    llvm::AllocaInst *variable = variableDetails->Alloca;
    if (variable == nullptr) {
        return logError("Unknown variable name: " + node->Id);
    }

    llvm::Value *index = node->Index->Accept(this);
    if (index->getType() != llvm::Type::getInt32Ty(*TheContext)) {
        return logError("Index should be an integer value");
    }

    createInBoundsCheck(variableDetails, index);

    llvm::Value *arrayElementPtr = Builder->CreateInBoundsGEP(
        variable->getType()->getPointerElementType(), variable, index);

    return Builder->CreateLoad(
        variable->getAllocatedType(), arrayElementPtr, node->Id + "Access_");
}

llvm::Value *CodegenNodeVisitor::createInBoundsCheck(
    VariableDetails *varDetails, llvm::Value *index) {
    auto intType = llvm::Type::getInt32Ty(*TheContext);

    auto outOfBoundsCond = Builder->CreateICmpSGE(index,
        llvm::ConstantInt::getSigned(intType, varDetails->Size), "sgetmp");

    llvm::Function *func = Builder->GetInsertBlock()->getParent();

    llvm::BasicBlock *outOfBoundsBB =
        llvm::BasicBlock::Create(*TheContext, "outOfBounds", func);
    llvm::BasicBlock *inBoundsBB =
        llvm::BasicBlock::Create(*TheContext, "inBounds");

    Builder->CreateCondBr(outOfBoundsCond, outOfBoundsBB, inBoundsBB);

    Builder->SetInsertPoint(outOfBoundsBB);

    llvm::Value *outOfBoundsMsg = globalStrings.find("outOfBounds")->second;

    llvm::Function *exitWithError_fn = TheModule->getFunction("exitWithError");
    llvm::Function *printint = TheModule->getFunction("PrintInt");
    Builder->CreateCall(exitWithError_fn, {outOfBoundsMsg, index});
    Builder->CreateUnreachable();

    func->getBasicBlockList().push_back(inBoundsBB);
    Builder->SetInsertPoint(inBoundsBB);

    auto negativeIndexCond = Builder->CreateICmpSLT(
        index, llvm::ConstantInt::getSigned(intType, 0), "slttmp");

    llvm::BasicBlock *negativeIndexBB =
        llvm::BasicBlock::Create(*TheContext, "negativeIndex", func);
    llvm::BasicBlock *validIndexBB =
        llvm::BasicBlock::Create(*TheContext, "validIndex");

    Builder->CreateCondBr(negativeIndexCond, negativeIndexBB, validIndexBB);

    Builder->SetInsertPoint(negativeIndexBB);

    llvm::Value *negativeIndexMsg = globalStrings.find("negativeIndex")->second;

    Builder->CreateCall(exitWithError_fn, {negativeIndexMsg, index});
    Builder->CreateUnreachable();

    func->getBasicBlockList().push_back(validIndexBB);
    Builder->SetInsertPoint(validIndexBB);

    return nullptr;
}

llvm::Value *CodegenNodeVisitor::Visit(Constant *node) {
    if (node->Tok->Type == TokenEnum::Num) {
        auto num = std::static_pointer_cast<Number>(node->Tok);
        return llvm::ConstantInt::getSigned(
            llvm::Type::getInt32Ty(*TheContext), num->Value);
    } else if (node->Tok->Type == TokenEnum::Real) {
        auto real = std::static_pointer_cast<RealNum>(node->Tok);
        return llvm::ConstantFP::get(llvm::Type::getFloatTy(*TheContext),
            llvm::APFloat((float)real->Value));
    } else if (node->Tok->Type == TokenEnum::True) {
        return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*TheContext), 1);
    } else if (node->Tok->Type == TokenEnum::False) {
        return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*TheContext), 0);
    } else if (node->Tok->Type == TokenEnum::String) {
        auto str = std::static_pointer_cast<String>(node->Tok);
        return Builder->CreateGlobalStringPtr(str->Literal);
    }

    return logError("Unsupported constant");
}

llvm::Value *CodegenNodeVisitor::Visit(If *node) {
    llvm::Value *cond = node->Condition->Accept(this);

    if (cond->getType() != llvm::Type::getInt1Ty(*TheContext)) {
        return logError("Condition should be a boolean value");
    }

    llvm::Function *func = Builder->GetInsertBlock()->getParent();

    llvm::BasicBlock *ifBB = llvm::BasicBlock::Create(*TheContext, "if", func);
    llvm::BasicBlock *finallyBB =
        llvm::BasicBlock::Create(*TheContext, "afterIf");
    llvm::BasicBlock *elseBB;

    if (node->Else != nullptr) {
        elseBB = llvm::BasicBlock::Create(*TheContext, "else");
        Builder->CreateCondBr(cond, ifBB, elseBB);
    } else {
        Builder->CreateCondBr(cond, ifBB, finallyBB);
    }

    Builder->SetInsertPoint(ifBB);
    node->Block->Accept(this);

    Builder->CreateBr(finallyBB);
    ifBB = Builder->GetInsertBlock();

    if (node->Else != nullptr) {
        func->getBasicBlockList().push_back(elseBB);
        Builder->SetInsertPoint(elseBB);

        if (node->Else != nullptr) {
            node->Else->Accept(this);
        }

        Builder->CreateBr(finallyBB);
        elseBB = Builder->GetInsertBlock();
    }

    func->getBasicBlockList().push_back(finallyBB);
    Builder->SetInsertPoint(finallyBB);

    return nullptr;
}

llvm::Value *CodegenNodeVisitor::Visit(ForStmt *node) {
    scope->NewScope();

    if (node->Init != nullptr) {
        llvm::Value *initVal = node->Init->Accept(this);
    }

    llvm::Function *func = Builder->GetInsertBlock()->getParent();
    llvm::BasicBlock *condBB;
    if (node->Cond != nullptr) {
        condBB = llvm::BasicBlock::Create(*TheContext, "cond", func);
    }
    llvm::BasicBlock *loopBB =
        llvm::BasicBlock::Create(*TheContext, "loop", func);
    llvm::BasicBlock *afterBB =
        llvm::BasicBlock::Create(*TheContext, "afterLoop");
    loopStack.push(afterBB);

    if (node->Cond != nullptr) {
        Builder->CreateBr(condBB);
        Builder->SetInsertPoint(condBB);
        llvm::Value *cond = node->Cond->Accept(this);
        Builder->CreateCondBr(cond, loopBB, afterBB);
    } else {
        Builder->CreateBr(loopBB);
    }

    Builder->SetInsertPoint(loopBB);
    node->Block->Accept(this);
    if (node->Post != nullptr) {
        node->Post->Accept(this);
    }

    if (node->Cond != nullptr) {
        Builder->CreateBr(condBB);
    } else {
        Builder->CreateBr(loopBB);
    }
    func->getBasicBlockList().push_back(afterBB);
    Builder->SetInsertPoint(afterBB);

    loopStack.pop();
    scope->EndScope();
    return nullptr;
}

llvm::Value *CodegenNodeVisitor::Visit(BreakStmt *node) {
    if (loopStack.empty()) {
        return logError("No loop to break from");
    }
    llvm::Function *func = Builder->GetInsertBlock()->getParent();
    llvm::BasicBlock *breakBB =
        llvm::BasicBlock::Create(*TheContext, "break", func);
    llvm::BasicBlock *restBB =
        llvm::BasicBlock::Create(*TheContext, "loopContued");

    llvm::Value *cond =
        llvm::ConstantInt::get(llvm::Type::getInt1Ty(*TheContext), 1);
    Builder->CreateCondBr(cond, breakBB, restBB);

    Builder->SetInsertPoint(breakBB);
    Builder->CreateBr(loopStack.top());

    func->getBasicBlockList().push_back(restBB);
    Builder->SetInsertPoint(restBB);

    return nullptr;
}

llvm::Value *CodegenNodeVisitor::Visit(ContinueStmt *node) {
    return logError("Continue is not supported");
}

std::string getTypeString(llvm::Type *type) {
    std::string typeStr;
    llvm::raw_string_ostream rso(typeStr);
    type->print(rso);
    rso.flush();
    return typeStr;
}

llvm::Value *CodegenNodeVisitor::Visit(FuncStmt *node) {
    std::vector<llvm::Type *> argTypes;
    for (auto &type : node->Arguments) {
        argTypes.push_back(getBasicType(type.first));
    }

    std::string funcName = node->Name;
    if (funcName != "main") {
        funcName = mangleFunction(funcName, argTypes);
    }

    {
        llvm::Function *f = TheModule->getFunction(funcName);
        if (f != nullptr) {
            return logError("Function cannot be redeclared");
        }
    }

    scope->NewScope();

    llvm::Type *returnType;
    if (funcName == "main") {
        node->ReturnType = std::make_unique<std::string>("int");
    }
    if (node->ReturnType != nullptr) {
        returnType = getBasicType(*node->ReturnType);
    } else {
        returnType = llvm::Type::getVoidTy(*TheContext);
    }

    llvm::FunctionType *funcType =
        llvm::FunctionType::get(returnType, argTypes, false);

    llvm::Function *func = llvm::Function::Create(
        funcType, llvm::Function::ExternalLinkage, funcName, TheModule.get());

    unsigned i = 0;
    for (auto &Arg : func->args()) {
        Arg.setName(node->Arguments[i++].second);
    }

    llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "body", func);
    Builder->SetInsertPoint(BB);

    for (auto &Arg : func->args()) {
        llvm::AllocaInst *alloca = createEntryBlockAlloca(
            func, Arg.getType(), nullptr, Arg.getName().str());

        Builder->CreateStore(&Arg, alloca);

        scope->Put(Arg.getName().str(), alloca);
    }

    node->Block->Accept(this);

    if (funcName == "main") {
        // auto alloca = NamedValues.find("z")->second;
        // auto ret = Builder->CreateLoad(alloca->getAllocatedType(), alloca,
        // "z"); Builder->CreateRet(ret);
        llvm::Value *retVal = Builder->getInt32(0);
        Builder->CreateRet(retVal);
    } else if (node->ReturnType == nullptr) {
        Builder->CreateRetVoid();
    } else {
        returnType = getBasicType(*node->ReturnType);
        llvm::Value *retVal = llvm::Constant::getNullValue(returnType);
        Builder->CreateRet(retVal);
    }

    std::string error_str;
    llvm::raw_string_ostream error_stream(error_str);
    if (llvm::verifyFunction(*func, &error_stream)) {
        std::string msg =
            "Error: Verification of the function " + funcName + " failed!\n";
        error_stream.flush();
        msg += error_str;
        logError(msg);
    }

    scope->EndScope();

    return func;
}

llvm::Value *CodegenNodeVisitor::Visit(ReturnStmt *node) {
    llvm::Function *func = Builder->GetInsertBlock()->getParent();
    llvm::BasicBlock *retBB =
        llvm::BasicBlock::Create(*TheContext, "return", func);
    llvm::BasicBlock *restBB =
        llvm::BasicBlock::Create(*TheContext, "funcContinued");

    llvm::Value *cond =
        llvm::ConstantInt::get(llvm::Type::getInt1Ty(*TheContext), 1);
    Builder->CreateCondBr(cond, retBB, restBB);

    Builder->SetInsertPoint(retBB);
    if (func->getName() == "main") {
        llvm::Value *retVal = Builder->getInt32(0);
        Builder->CreateRet(retVal);
    } else if (node->Expr == nullptr) {
        Builder->CreateRetVoid();
    } else {
        auto retVal = node->Expr->Accept(this);
        Builder->CreateRet(retVal);
    }

    func->getBasicBlockList().push_back(restBB);
    Builder->SetInsertPoint(restBB);

    return nullptr;
}

llvm::Value *CodegenNodeVisitor::Visit(Call *node) {
    std::vector<llvm::Value *> argVals;
    std::vector<llvm::Type *> argTypes;
    for (unsigned i = 0, e = node->Args.size(); i != e; ++i) {
        argVals.push_back(node->Args[i]->Accept(this));
        if (argVals.back() == nullptr) {
            return logError("Invalid arguments in the func call");
        }
        argTypes.push_back(argVals.back()->getType());
    }

    std::string funcName = node->FuncName;
    if (funcName != "printf") {
        funcName = mangleFunction(funcName, argTypes);
    }

    llvm::Function *func = TheModule->getFunction(funcName);
    if (func == nullptr) {
        return logError("Unknown function referenced: " + node->FuncName +
                        " Mangled: " + funcName);
    }

    if (func->arg_size() > node->Args.size() && !func->isVarArg()) {
        return logError("Incorrect number of arguments passed. For " +
                        node->FuncName +
                        ". Got: " + std::to_string(node->Args.size()) +
                        " Expected: " + std::to_string(func->arg_size()));
    }

    return Builder->CreateCall(func, argVals);
}

llvm::Value *CodegenNodeVisitor::Visit(ExpressionStmt *node) {
    return node->Expr->Accept(this);
}

llvm::Value *CodegenNodeVisitor::Visit(BlockStmt *node) {
    scope->NewScope();
    for (auto const &i : node->Statements) {
        llvm::Value *v = i->Accept(this);
    }

    scope->EndScope();
    return nullptr;
}

llvm::AllocaInst *CodegenNodeVisitor::createEntryBlockAlloca(
    llvm::Function *func, llvm::Type *type, llvm::Value *arraySize,
    const std::string &varName) {
    llvm::IRBuilder<> TmpB(
        &func->getEntryBlock(), func->getEntryBlock().begin());

    return TmpB.CreateAlloca(type, arraySize, varName);
}

std::string CodegenNodeVisitor::mangleFunction(
    std::string name, std::vector<llvm::Type *> args) {
    std::string typesStr;
    llvm::raw_string_ostream stream(typesStr);
    for (auto &arg : args) {
        arg->print(stream);
        stream << "_";
    }
    stream.flush();
    return name + "__" + typesStr;
}

llvm::Type *CodegenNodeVisitor::getBasicType(std::string type) {
    if (type == "int") {
        return llvm::Type::getInt32Ty(*TheContext);
    } else if (type == "float") {
        return llvm::Type::getFloatTy(*TheContext);
    } else if (type == "bool") {
        return llvm::Type::getInt1Ty(*TheContext);
    } else if (type == "string") {
        return llvm::Type::getInt8PtrTy(*TheContext);
    } else {
        logError("Unsupported arg type: " + type);
        return nullptr;
    }
}

llvm::Value *CodegenNodeVisitor::logError(std::string msg) {
    errors << msg << std::endl;
    std::cerr << msg << std::endl;
    return nullptr;
}

int CodegenNodeVisitor::Compile(Program *prog, std::string fileName) {
    prog->Accept(this);

    // auto alloca = NamedValues.find("z")->second;
    // auto ret = Builder->CreateLoad(alloca->getAllocatedType(), alloca, "z");
    // Builder->CreateRet(ret);

    llvm::outs() << "\n";
    TheModule->print(llvm::errs(), nullptr);

    std::string error_str;
    llvm::raw_string_ostream error_stream(error_str);
    if (llvm::verifyModule(*TheModule, &error_stream)) {
        std::string msg = "Error: Verification of the module failed!\n";
        error_stream.flush();
        msg += error_str;
        logError(msg);
    }

    if (errors.str() != "") {
        llvm::errs() << "\n" << errors.str() << "\n";
        return 1;
    }

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

    std::string ObjectFilename = fileName + ".o";
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

    std::string binaryFilename = fileName + ".a";
    std::string externalFunctions =
        " /home/sardar/code/thesis/pepel/external_functions/Basic.o ";
    std::string cmd = "clang++ -no-pie  " + ObjectFilename + " -o " +
                      binaryFilename + externalFunctions + " && rm " +
                      ObjectFilename;
    std::system(cmd.c_str());

    // llvm::outs() << "\nWrote " << binaryFilename << "\n";

    return 0;
}
