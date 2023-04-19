#pragma once
#include <string>

#include "llvm/IR/Value.h"

class Node {
   public:
    virtual ~Node() = default;
    virtual llvm::Value* codegen() = 0;
    virtual std::string ToString() = 0;
};
