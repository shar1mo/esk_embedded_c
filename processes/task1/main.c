#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
  int status;
  pid_t child_pid;

  child_pid = fork();
  if(child_pid == 0) {
    printf("child pid: %d\nchild ppid: %d\n\n", getpid(), getppid());
    exit(EXIT_SUCCESS);
  } else {
    printf("parent pid: %d\nparant ppid: %d\n\n", getpid(), getppid());
    wait(&status);
    printf("status: %d\n", WEXITSTATUS(status));
  }
  
  return EXIT_SUCCESS;
}