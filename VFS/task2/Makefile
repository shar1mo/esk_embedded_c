BIN := mc_fork
SRC := main.c
OBJ := main.o
CC := gcc
CFLAGS := -fPIC -Wall -Wextra -g
LDFLAGS :=  -lncurses

.PHONY: clean, run

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(CFLAGS) $(LDFLAGS)

$(OBJ): $(SRC)
	$(CC) $(SRC) -c

clean:
	rm -rf *.o $(BIN)

run:
	./$(BIN)