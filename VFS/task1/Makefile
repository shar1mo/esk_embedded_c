BIN := test
SRC := file_input_output.c
OBJ := file_input_output.o
CC := gcc
FILE := output.txt

.PHONY: clean run

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN)

$(OBJ): $(SRC)
	$(CC) $(SRC) -c

clean:
	rm -rf *.o $(BIN) $(FILE)

run:
	./$(BIN)