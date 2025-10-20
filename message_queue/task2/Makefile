BIN := server client
SRC := server.c client.c
OBJ := $(SRC:.c=.o)
CC := gcc
CFLAGS := -Wall -Wextra -g -pthread -lncurses -lrt

.PHONY: all clean

all: $(BIN)

server: server.c
	$(CC) $< -o $@ $(CFLAGS)

client: client.c
	$(CC) $< -o $@ $(CFLAGS)

clean:
	rm -f $(BIN) *.o
