BIN := test
SRC := main.c
OBJ := main.o
CC := gcc
CFLAGS := -Wall -Wextra -g
LDFLAGS := -lpthread

.PHONY: clean, run

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(CFLAGS) $(LDFLAGS)

$(OBJ): $(BIN)
	$(CC) $(SRC) -c

run:
	./$(BIN)

clean:
	rm -rf $(BIN) *.o
