exp:
	clang++ -std=c++17 -g exp.cpp `llvm-config-14 --cxxflags --ldflags --system-libs --libs all` -o exp.a 

expc:
	clang++ -std=c++17 -g exp_c.cpp `llvm-config-14 --cxxflags --ldflags --system-libs --libs all` -o expc.a && ./expc.a
#clang++ -no-pie  output.o -o output.a && ./output.a
