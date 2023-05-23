#pragma once
#include <map>
#include <memory>
#include <string>

#include "llvm/IR/Instructions.h"

class VariableDetails {
   public:
    llvm::AllocaInst *Alloca;
    const int Size;
};

class Env {
   public:
    std::map<std::string, VariableDetails> Table;
    std::shared_ptr<Env> Parent;

    Env(std::shared_ptr<Env> parent) : Parent(parent) {
    }

    VariableDetails *Get(std::string varName) {
        auto var = Table.find(varName);
        if (var != Table.end()) {
            return &var->second;
        }
        if (Parent != nullptr) {
            return Parent->Get(varName);
        }
        return nullptr;
    }

    void Put(std::string varName, llvm::AllocaInst *var, const int size) {
        Table.insert({varName, {var, size}});
    }
};

class Scope {
    std::shared_ptr<Env> env;

   public:
    Scope() : env(std::make_shared<Env>(nullptr)) {
    }

    void NewScope() {
        env = std::make_shared<Env>(env);
    }

    void EndScope() {
        env = env->Parent;
    }

    VariableDetails *Get(std::string varName) {
        return env->Get(varName);
    }
    void Put(std::string varName, llvm::AllocaInst *var, const int size = 1) {
        env->Put(varName, var, size);
    }
};
