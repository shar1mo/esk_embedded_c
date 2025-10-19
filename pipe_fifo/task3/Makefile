BIN := myshell
SRC := myshell.c
OBJ := $(SRC:.c=.o)
CC := gcc
CFLAGS := -Wall -Wextra -g

.PHONY: all run clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm -rf *.o $(BIN)
