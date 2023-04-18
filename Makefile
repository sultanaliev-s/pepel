all:
	clang++ -std=c++17 -g -O3 main.cpp lexer/*.cpp parser/*.cpp `llvm-config-14 --cxxflags --ldflags --system-libs --libs all` -o pepel.a && ./pepel.a
#-Wall -Wextra -Wpedantic 

exp:
	clang++ -std=c++17 -g exp.cpp `llvm-config-14 --cxxflags --ldflags --system-libs --libs all` -o exp.a 

expc:
	clang++ -std=c++17 -g exp_c.cpp `llvm-config-14 --cxxflags --ldflags --system-libs --libs all` -o expc.a && ./expc.a
#clang++ -no-pie  output.o -o output.a && ./output.a
