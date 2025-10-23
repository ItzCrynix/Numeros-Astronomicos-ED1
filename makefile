CC := gcc
CCFLAGS := -w

build: bigint.h list.h
	$(CC) *.c -o main $(CCFLAGS)

run:
	./main

