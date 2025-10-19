#include "fifohd.h"

int main(void) 
{
  int fd;
  char *myfifo = "/tmp/myfifo";

  if(mkfifo(myfifo, 0600) == -1) {
    perror("mkfifo");
    return EXIT_FAILURE;
  };
  printf("server start and write in buf\n");

  char buf_tx[BUFFER_SIZE] = "Hi!";
  fd = open(myfifo, O_WRONLY);
  if(fd == -1) {
    perror("open");
    return EXIT_FAILURE;
  }
  ssize_t n = write(fd, buf_tx, strlen(buf_tx) + 1);
  if(n == -1) {
    perror("write");
    return EXIT_FAILURE;
  }
  close(fd);

  printf("server stopped\n");
  return EXIT_SUCCESS;
}