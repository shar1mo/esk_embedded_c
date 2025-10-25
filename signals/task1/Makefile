BIN_RECEIVER := receiver
BIN_SENDER := sender
SRC_RECEIVER := receiver.c
SRC_SENDER := sender.c
OBJ_RECEIVER := receiver.o
OBJ_SENDER := sender.o
CC := gcc

.PHONY: clean all

all: $(BIN_RECEIVER) $(BIN_SENDER)

$(BIN_RECEIVER): $(OBJ_RECEIVER)
	$(CC) $(OBJ_RECEIVER) -o $(BIN_RECEIVER)

$(OBJ_RECEIVER): $(SRC_RECEIVER)
	$(CC) $(SRC_RECEIVER) -c

$(BIN_SENDER): $(OBJ_SENDER)
	$(CC) $(OBJ_SENDER) -o $(BIN_SENDER)

$(OBJ_SENDER): $(SRC_SENDER)
	$(CC) $(SRC_SENDER) -c

clean:
	rm -rf *.o $(BIN_RECEIVER) $(BIN_SENDER)

run_receiver:
	./$(BIN_RECEIVER)

run_sender:
	./$(BIN_SENDER) $$(pidof $(BIN_RECEIVER))