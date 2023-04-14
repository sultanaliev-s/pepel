all:
	g++ -std=c++17 -Wall -Wextra -Wpedantic -o main main.cpp lexer/*.cpp
