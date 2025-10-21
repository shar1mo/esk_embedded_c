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
    
    shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    
    shm_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    
    while (strlen(shm_ptr) == 0) {
        sleep(1);
    }
    
    printf("[CLIENT] Received: %s\n", shm_ptr);
    
    strcpy(shm_ptr, "Hello!");
    printf("[CLIENT] Sent: Hello!\n");
    
    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);
    
    return EXIT_SUCCESS;
}