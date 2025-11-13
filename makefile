CC := gcc
CCFLAGS := -Wall -Wpedantic -Wextra -O3 -march=native 

.PHONY: all build run

all: build run

build: bigInt.h
	$(CC) *.c -o main $(CCFLAGS)

run:
	./main

