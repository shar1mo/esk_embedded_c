BIN_LOOP := event_loop
BIN_SENDER := sender
SRC_LOOP := event_loop.c
SRC_SENDER := sender.c
OBJ_LOOP := event_loop.o
OBJ_SENDER := sender.o
CC := gcc

.PHONY: clean all

all: $(BIN_LOOP) $(BIN_SENDER)

$(BIN_LOOP): $(OBJ_LOOP)
	$(CC) $(OBJ_LOOP) -o $(BIN_LOOP)

$(OBJ_LOOP): $(SRC_LOOP)
	$(CC) $(SRC_LOOP) -c

$(BIN_SENDER): $(OBJ_SENDER)
	$(CC) $(OBJ_SENDER) -o $(BIN_SENDER)

$(OBJ_SENDER): $(SRC_SENDER)
	$(CC) $(SRC_SENDER) -c

clean:
	rm -rf *.o $(BIN_LOOP) $(BIN_SENDER)

run_loop:
	./$(BIN_LOOP)

run_sender:
	./$(BIN_SENDER) $$(pidof $(BIN_LOOP))