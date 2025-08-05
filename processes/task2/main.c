#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
  int status;
  pid_t child_pid1, child_pid2;

  child_pid1 = fork();
  if(child_pid1 == 0) {
    pid_t child_pid3, child_pid4;
    child_pid3 = fork();
    if(child_pid3 == 0) {
      printf("child pid 3: %d\nchild ppid 3: %d\n\n", getpid(), getppid());
      exit(EXIT_SUCCESS);
    } else {
      child_pid4 = fork();
      if(child_pid4 == 0) {
        printf("child pid 4: %d\nchild ppid 4: %d\n\n", getpid(), getppid());
        exit(EXIT_SUCCESS);
      } else {
        waitpid(child_pid3, &status, 0);
        waitpid(child_pid4, &status, 0);
        printf("child pid 1: %d\nchild ppid 1: %d\n\n", getpid(), getppid());
        exit(EXIT_SUCCESS);
      }
    }
  } else {
    child_pid2 = fork();
    if(child_pid2 == 0) {
      pid_t child_pid5;
      child_pid5 = fork();
      if(child_pid5 == 0) {
        printf("child pid 5: %d\nchild ppid 5: %d\n\n", getpid(), getppid());
        exit(EXIT_SUCCESS);
      } else {
        waitpid(child_pid5, &status, 0);
        printf("child pid 2: %d\nchild ppid 2: %d\n\n", getpid(), getppid());
        exit(EXIT_SUCCESS);
      }
    } else {
      waitpid(child_pid1, &status, 0);
      waitpid(child_pid2, &status, 0);
      printf("parent pid: %d\nparant ppid: %d\n\n", getpid(), getppid());
      printf("status: %d\n", status);
    }
  }

  return EXIT_SUCCESS;
}