#define _POSIX_C_SOURCE 200112L
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SHM_SIZE 1024
#define SHM_KEY 0x1234

int main() {
    int shmid;
    char *shm_ptr;
    
    shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    
    shm_ptr = (char*)shmat(shmid, NULL, 0);
    if (shm_ptr == (char*)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    strcpy(shm_ptr, "Hi!");
    printf("[SERVER] Sent: Hi!\n");
    printf("[SERVER] Waiting for response...\n");
    
    while (strcmp(shm_ptr, "Hi!") == 0) {
        sleep(1);
    }
    
    printf("[SERVER] Received: %s\n", shm_ptr);
    
    shmdt(shm_ptr);
    shmctl(shmid, IPC_RMID, NULL);
    
    printf("[SERVER] Shared memory deleted\n");
    return EXIT_SUCCESS;
}