BIN := test
SRC := main.c add.c init.c print_all.c find_name.c delete.c
OBJ := main.o add.o init.o print_all.o find_name.o delete.o
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