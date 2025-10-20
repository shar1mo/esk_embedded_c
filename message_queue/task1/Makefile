BIN := server_sysv client_sysv server_posix client_posix
SRC := $(BIN:=.c)
OBJ := $(SRC:.c=.o)
CC := gcc
CFLAGS := -Wall -Wextra -g -lrt

.PHONY: all clean run-sysv run-posix

all: $(BIN)

%: %.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run-sysv: server_sysv client_sysv
	@echo "=== Running System V example ==="
	@./server_sysv & sleep 0.5; ./client_sysv

run-posix: server_posix client_posix
	@echo "=== Running POSIX example ==="
	@./server_posix & sleep 2; ./client_posix

clean:
	rm -rf *.o $(BIN)
