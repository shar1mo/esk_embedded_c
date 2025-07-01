BIN := test
SRC := main.c add.c sub.c mul.c div.c
OBJ := main.o add.o sub.o mul.o div.o
HDR := libcalc.h
LIB := libcalc.a
CC := gcc

.PHONY: clean, run, lib

$(BIN): $(OBJ) $(LIB)
	$(CC) main.o -o $(BIN) -L. -lcalc

%.o: %.c $(HDR)
	$(CC) -c $< -o $@

lib: add.o sub.o mul.o div.o
	ar rc $(LIB) add.o sub.o mul.o div.o

clean:
	rm -rf  $(BIN) *.o

run:
	./$(BIN)