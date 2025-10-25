#define _POSIX_C_SOURCE 200112L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    sigset_t wait_set;
    int sig;
    
    sigemptyset(&wait_set);
    sigaddset(&wait_set, SIGUSR1);
    
    if (sigprocmask(SIG_BLOCK, &wait_set, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }
    
    printf("[EVENT_LOOP] PID: %d\n", getpid());
    printf("[EVENT_LOOP] SIGUSR1 is blocked and will be handled via sigwait()\n");
    printf("[EVENT_LOOP] Waiting for SIGUSR1 signals...\n");
    
    while (1) {
        if (sigwait(&wait_set, &sig) == 0) {
            printf("[EVENT_LOOP] Received signal: %d\n", sig);
            printf("[EVENT_LOOP] Continuing to next iteration...\n");
        }
    }
    
    return EXIT_SUCCESS;
}