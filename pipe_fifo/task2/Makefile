BIN := server client
SRC := server.c client.c
OBJ := $(SRC:.c=.o)
HD := fifohd.h
CC := gcc
CFLAGS := -Wall -Wextra -g

.PHONY: all run-server run-client clean

all: $(BIN)

server: server.o
	$(CC) $^ -o $@ $(CFLAGS)

client: client.o
	$(CC) $^ -o $@ $(CFLAGS)

%.o: %.c $(HD)
	$(CC) $(CFLAGS) -c $< -o $@

run-server: server
	./server

run-client: client
	./client

clean:
	rm -rf *.o $(BIN)

