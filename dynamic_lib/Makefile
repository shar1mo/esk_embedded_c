BIN := test
SRC := main.c add.c sub.c mul.c div.c
OBJ := main.o add.o sub.o mul.o div.o
HDR := libcalc.h
LIB := libcalc.so
CC := gcc
CFLAGS := -fPIC -Wall -Wextra

.PHONY: clean run lib

$(BIN): main.o $(LIB)
	$(CC) main.o -o $(BIN) -L. -lcalc

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

lib: add.o sub.o mul.o div.o
	$(CC) -shared add.o sub.o mul.o div.o -o $(LIB)

clean:
	rm -rf $(BIN) *.o $(LIB)

run:
	LD_DEBUG=libs LD_LIBRARY_PATH=. ./$(BIN)