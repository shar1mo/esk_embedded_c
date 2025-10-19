BIN := pipe
SRC := pipe.c
OBJ := pipe.o
CC := gcc
CFLAGS := -Wall -Wextra -g

.PHONY: clean, run

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(CFLAGS)

$(OBJ): $(SRC)
	$(CC) $(SRC) -c

run:
	./$(BIN)

clean:
	rm -rf *.o $(BIN)