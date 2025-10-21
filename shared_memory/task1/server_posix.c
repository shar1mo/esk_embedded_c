#define _POSIX_C_SOURCE 200112L
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define SHM_NAME "/chat_shm"
#define SHM_SIZE 1024

int main() {
    int shm_fd;
    char *shm_ptr;
    
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    
    ftruncate(shm_fd, SHM_SIZE);
    
    shm_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    
    strcpy(shm_ptr, "Hi!");
    printf("[SERVER] Sent: Hi!\n");
    printf("[SERVER] Waiting for response...\n");
    
    while (strcmp(shm_ptr, "Hi!") == 0) {
        sleep(1);
    }
    
    printf("[SERVER] Received: %s\n", shm_ptr);
    
    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);
    shm_unlink(SHM_NAME);
    
    printf("[SERVER] Shared memory deleted\n");
    return EXIT_SUCCESS;
}