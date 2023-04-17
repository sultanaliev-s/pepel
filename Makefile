all:
	clang++ -std=c++17 -g -O3 main.cpp lexer/*.cpp parser/*.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core orcjit native` -o main
#-Wall -Wextra -Wpedantic 
