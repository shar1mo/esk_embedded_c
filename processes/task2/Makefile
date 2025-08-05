BIN := test
SRC := main.c
OBJ := main.o
CC := gcc
CFLAGS := -Wall -Wextra -g

.PHONY: clean, run

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(CFLAGS)

$(OBJ): $(SRC)
	$(CC) $(SRC) -c

clean:
	rm -rf *.o $(BIN)

run:
	./$(BIN)
