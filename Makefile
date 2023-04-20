BINARY=pepel.a
CODE=./ast ./codegen ./lexer ./parser

CC=clang++
OPT=-O0
DEPFLAGS=-MP -MD
LLVM=`llvm-config-14 --cxxflags --ldflags --system-libs --libs all`
CFLAGS=-std=c++17 -g $(foreach D,$(CODE),-I$(D)) $(OPT) $(DEPFLAGS) $(LLVM)
#-Wall -Wextra -Wpedantic 

CFILES=$(foreach D,$(CODE),$(wildcard $(D)/*.cpp)) main.cpp
OBJECTS=$(patsubst %.cpp,%.o,$(CFILES))
DEPFILES=$(patsubst %.cpp,%.d,$(CFILES))

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^



%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)

check:
	./pepel.a && ./myprog.a
	@echo $$?	
