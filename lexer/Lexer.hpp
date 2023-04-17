#pragma once
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "Number.hpp"
#include "RealNum.hpp"
#include "Token.hpp"
#include "TokenEnum.hpp"
#include "Word.hpp"

class Lexer {
   public:
    int Line;

   private:
    int peek;
    std::ifstream fStream;
    std::shared_ptr<Token> lastToken;

   public:
    Lexer(std::string filePath);
    std::shared_ptr<Token> Scan();

   private:
    std::shared_ptr<Token> scan();
    bool readChar(int ch);
    void readChar();
    int charToInt(char x);
    bool requiresSemicolon();
};
