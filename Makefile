BINARY=pepel.a
CODE=. ./ast ./codegen ./lexer ./parser

CC=clang++
OPT=-O0
DEPFLAGS=-MP -MD
INCLUDE=-I/usr/include/llvm-c-14 -I/usr/include/llvm-14
LLVM=`llvm-config-14 --cxxflags --ldflags --system-libs --libs all`
CFLAGS=-std=c++17 -g $(foreach D,$(CODE),-I$(D)) $(INCLUDE) $(OPT) $(LLVM)
#-Wall -Wextra -Wpedantic 

CFILES=$(foreach D,$(CODE),$(wildcard $(D)/*.cpp))
OBJECTS=$(patsubst %.cpp,%.o,$(CFILES))
DEPFILES=$(patsubst %.cpp,%.d,$(CFILES))

#all: $(BINARY)
all:
	clang++ -std=c++17 -g -O3 main.cpp lexer/*.cpp parser/*.cpp `llvm-config-14 --cxxflags --ldflags --system-libs --libs all` -o pepel.a && ./pepel.a

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)
	
