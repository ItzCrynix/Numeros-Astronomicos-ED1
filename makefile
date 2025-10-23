CC := gcc
CCFLAGS := -w

build: bigint.h
	$(CC) *.c -o main $(CCFLAGS)

run:
	./main

