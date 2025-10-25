#define _POSIX_C_SOURCE 200112L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sigusr1_handler(int sig) {
    printf("[RECEIVER] Received SIGUSR1 signal\n");
}

int main() {
    struct sigaction sa;
    
    sa.sa_handler = sigusr1_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    
    printf("[RECEIVER] PID: %d\n", getpid());
    printf("[RECEIVER] Waiting for SIGUSR1 signals...\n");
    
    while (1) {
        pause();
    }
    
    return EXIT_SUCCESS;
}