#define _POSIX_C_SOURCE 200112L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    pid_t pid = atoi(argv[1]);
    
    printf("[SENDER] Sending SIGINT to process %d\n", pid);
    
    if (kill(pid, SIGINT) == -1) {
        perror("kill");
        exit(EXIT_FAILURE);
    }
    
    printf("[SENDER] Signal sent successfully\n");
    printf("[SENDER] Note: The signal is blocked and won't terminate the process\n");
    
    return EXIT_SUCCESS;
}