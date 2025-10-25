#define _POSIX_C_SOURCE 200112L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    sigset_t block_set;
    
    sigemptyset(&block_set);
    sigaddset(&block_set, SIGINT);
    
    if (sigprocmask(SIG_BLOCK, &block_set, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }
    
    printf("[BLOCKER] PID: %d\n", getpid());
    printf("[BLOCKER] SIGINT is blocked. Use 'kill -SIGINT %d' to test\n", getpid());
    printf("[BLOCKER] Entering infinite loop...\n");
    
    while (1) {
        sleep(1);
    }
    
    return EXIT_SUCCESS;
}