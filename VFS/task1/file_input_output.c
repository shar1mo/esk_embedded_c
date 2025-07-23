#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void) {
  const char string[] = "String from file";
  int fd = open("output.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
  
  if(fd == -1) {
    perror("open or create failed\n");
    return EXIT_FAILURE;
  } else {
    printf("file created/opened successfully\n\n");
  }

  printf("write to output.txt: String from file\n\n");
  ssize_t bytes_written = write(fd, string, sizeof(string) - 1);

  if(bytes_written == -1) {
    perror("write failed\n");
    close(fd);
    return EXIT_FAILURE;
  }

  if(close(fd) == -1) {
    perror("closed failed\n");
    return EXIT_FAILURE;
  }

  fd = open("output.txt", O_RDWR);

  if(fd == -1) {
    perror("open failed\n");
    return EXIT_FAILURE;
  } else {
    printf("file opened successfully\n");
  }
  
  struct stat file_info;
  if(fstat(fd, &file_info) == -1) {
    perror("stat failed\n");
    close(fd);
    return EXIT_FAILURE;
  }

  printf("reverse reading from output.txt: ");
  for(off_t pos = file_info.st_size - 1; pos >= 0; pos--) {
    char c;
    if(lseek(fd, pos, SEEK_SET) == -1) {
      perror("lseek failed\n");
      return EXIT_FAILURE;
    }

    if(read(fd, &c, 1) != 1) {
      perror("read failed\n");
      close(fd);
      return EXIT_FAILURE;
    }

    putchar(c);
  }

  putchar('\n');
  close(fd);
  return EXIT_SUCCESS;
}