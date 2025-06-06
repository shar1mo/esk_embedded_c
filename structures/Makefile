BIN := test
SRC := main.c
OBJ := test.o
CC := gcc

.PHONY: clean, run

$(BIN): $(OBJ)
	$(CC) $(SRC) -o $(BIN)

$(OBJ): $(SRC)
	$(CC) $(SRC) -c

clean:
	rm -rf $(BIN) *.o

run:
	./$(BIN)