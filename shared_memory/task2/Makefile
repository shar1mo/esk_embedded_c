CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = 
LIBS = 
NCURSES_LIBS = -lncurses
PTHREAD_LIBS = -lpthread

all: chat_server chat_client

chat_server: chat_server.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(LIBS)

chat_client: chat_client.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(NCURSES_LIBS) $(PTHREAD_LIBS) $(LIBS)

clean:
	rm -f chat_server chat_client

clean_shm:
	-ipcrm -a 2>/dev/null || true

run_server: chat_server
	./chat_server

run_client: chat_client
	./chat_client

.PHONY: all clean clean_shm run_server run_client