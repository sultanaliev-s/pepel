#pragma once
#include <string>

class Node {
   public:
    virtual ~Node() = default;
    virtual std::string ToString() {
        return "node";
    };
};
