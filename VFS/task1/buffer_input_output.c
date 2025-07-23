#include <stdio.h>
#include <stdlib.h>

int main(void) {
  const char str[] = "String from file";
  FILE* ptr_file;
  
  ptr_file = fopen("output.txt", "w");
  if (ptr_file == NULL) {
    perror("error creating a file\n");
    return EXIT_FAILURE;
  } else {
    printf("file created\n\n");
  }

  printf("writing to output.txt: String from file\n\n");
  fputs(str, ptr_file);
  fclose(ptr_file);
  
  ptr_file = fopen("output.txt", "r");
  if(ptr_file == NULL) {
    perror("error open file\n");
    return EXIT_FAILURE;
  } else {
    printf("reverse reading from output.txt: ");
  }
  
  fseek(ptr_file, 0, SEEK_END);
  long size_f = ftell(ptr_file);
  
  while (size_f-- > 0) {
    fseek(ptr_file, size_f, SEEK_SET);
    int c = fgetc(ptr_file);
    if(c == EOF) break;
    putchar(c);
  }
  putchar('\n');

  fclose(ptr_file);
  return EXIT_SUCCESS;
}