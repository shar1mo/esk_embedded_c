#include "fifohd.h"

int main(void) 
{
  int fd;
  char *myfifo = "/tmp/myfifo";
  char buf_rx[BUFFER_SIZE] = "'empty'";

  printf("buf_rx before reading: %s\n", buf_rx);

  fd = open(myfifo, O_RDONLY);
  if(fd == -1) {
    perror("open");
    return EXIT_FAILURE;
  }
  ssize_t n = read(fd, buf_rx, sizeof(buf_rx));
  if(n == -1) {
    perror("read");
    return EXIT_FAILURE;
  }
  printf("buf_rx after reading: %s\n", buf_rx);
  close(fd);

  if (unlink(myfifo) == -1) {
     perror("unlink");
  } else {
    printf("client: fifo removed\n");
  }

  return EXIT_SUCCESS;
}