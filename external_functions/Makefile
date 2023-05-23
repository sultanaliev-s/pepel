CODE=./

CC=clang++
OPT=-O0
WARNINGS=-Wall -Wextra -Wpedantic
CFLAGS=-std=c++17 -g $(foreach D,$(CODE),-I$(D)) $(OPT) $(WARNINGS)

CFILES=$(foreach D,$(CODE),$(wildcard $(D)/*.cpp))
OBJECTS=$(patsubst %.cpp,%.o,$(CFILES))

all: $(OBJECTS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJECTS)
