#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 256
#define FD_COUNT 2

int main(void)
{
  int fd[FD_COUNT];
  char buf_tx[BUFFER_SIZE] = "Hi!";
  char buf_rx[BUFFER_SIZE] = "'empty'";
  pid_t pid;
  int* status;

  if(pipe(fd)){
    perror("pipe error\n");
    return EXIT_FAILURE;
  }

  pid = fork();
  if(pid == 0) {
    close(fd[1]);
    printf("before read pipe: %s\n", buf_rx);
    read(fd[0], buf_rx, sizeof(buf_rx));
    printf("after read pipe: %s\n", buf_rx);
    exit(EXIT_SUCCESS);
  } else {
    close(fd[0]);
    printf("write in pipe\n");
    write(fd[1], buf_tx, strlen(buf_tx) + 1);
    waitpid(pid, status, 0);
  }

  return EXIT_SUCCESS;
}