BIN_BLOCKER := blocker
BIN_SENDER := sender
SRC_BLOCKER := blocker.c
SRC_SENDER := sender.c
OBJ_BLOCKER := blocker.o
OBJ_SENDER := sender.o
CC := gcc

.PHONY: clean all

all: $(BIN_BLOCKER) $(BIN_SENDER)

$(BIN_BLOCKER): $(OBJ_BLOCKER)
	$(CC) $(OBJ_BLOCKER) -o $(BIN_BLOCKER)

$(OBJ_BLOCKER): $(SRC_BLOCKER)
	$(CC) $(SRC_BLOCKER) -c

$(BIN_SENDER): $(OBJ_SENDER)
	$(CC) $(OBJ_SENDER) -o $(BIN_SENDER)

$(OBJ_SENDER): $(SRC_SENDER)
	$(CC) $(SRC_SENDER) -c

clean:
	rm -rf *.o $(BIN_BLOCKER) $(BIN_SENDER)

run_blocker:
	./$(BIN_BLOCKER)

run_sender:
	./$(BIN_SENDER) $$(pidof $(BIN_BLOCKER))