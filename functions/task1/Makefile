BIN := test
SRC := main.c add.c delete.c search.c print_all.c
OBJ := main.o add.o delete.o search.o print_all.o
CC := gcc

.PHONY: clean, run

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN)

$(OBJ): $(SRC) func.h
	$(CC) $(SRC) -c

clean:
	rm -rf $(BIN) *.o

run:
	./$(BIN)