#pragma once
#include <vector>

#include "Node.hpp"
#include "Statement.hpp"

class Program : public Node {
   public:
    std::vector<std::unique_ptr<Statement>> Statements;

    Program() : Statements(0) {
    }

    std::string ToString() override;
};
