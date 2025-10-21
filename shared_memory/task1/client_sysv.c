#define _POSIX_C_SOURCE 200112L
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024
#define SHM_KEY 0x1234

int main() {
    int shmid;
    char *shm_ptr;
    
    shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    
    shm_ptr = (char*)shmat(shmid, NULL, 0);
    if (shm_ptr == (char*)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    
    while (strlen(shm_ptr) == 0) {
        sleep(1);
    }
    
    printf("[CLIENT] Received: %s\n", shm_ptr);
    
    strcpy(shm_ptr, "Hello!");
    printf("[CLIENT] Sent: Hello!\n");
    
    shmdt(shm_ptr);
    
    return EXIT_SUCCESS;
}