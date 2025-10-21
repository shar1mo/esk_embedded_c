# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = 
LIBS = 
RT_LIBS = -lrt

all: sysv posix

sysv: server_sysv client_sysv

server_sysv: server_sysv.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(LIBS)

client_sysv: client_sysv.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(LIBS)

posix: server_posix client_posix

server_posix: server_posix.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(RT_LIBS) $(LIBS)

client_posix: client_posix.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(RT_LIBS) $(LIBS)

clean:
	rm -f server_sysv client_sysv server_posix client_posix

clean_shm:
	-ipcrm -a 2>/dev/null || true
	-rm -f /dev/shm/chat_shm 2>/dev/null || true

run_sysv_server: server_sysv
	./server_sysv

run_sysv_client: client_sysv
	./client_sysv

run_posix_server: server_posix
	./server_posix

run_posix_client: client_posix
	./client_posix

.PHONY: all sysv posix clean clean_shm run_sysv_server run_sysv_client run_posix_server run_posix_client