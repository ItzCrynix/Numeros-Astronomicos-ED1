CC	:=	gcc
CCFLAGS :=	-std=c11 -Wall -Wpedantic -Wextra -Werror -O3 -march=native
TARGET	:=	main
OBJS	:=	main.o bigInt.o

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

%.o: %.c bigInt.h
	$(CC) $(CFLAGS) -c $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

