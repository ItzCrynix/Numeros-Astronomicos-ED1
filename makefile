CC := gcc
CCFLAGS := -Wall -Wpedantic -Wextra 

.PHONY: all build run

all: build run

build: bigint.h
	$(CC) *.c -o main $(CCFLAGS)

run:
	./main

