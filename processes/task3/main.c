#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <limits.h>

#define MAX_NAME_LEN 256

void print_cwd() {
  char cwd[MAX_NAME_LEN];
  if(getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("%s$ ", cwd);
  } else {
    perror("getcwd() error\n");
    printf("$ ");
  }
}

int main(void) {
  int status;
  char line[MAX_NAME_LEN * 2];
  char command[MAX_NAME_LEN];
  char option[MAX_NAME_LEN] = "";
  pid_t bash_pid;

  while(1) {
    print_cwd();
    
    if(fgets(line, sizeof(line), stdin) == NULL) {
      perror("error input");
      exit(EXIT_FAILURE);
    }

    line[strcspn(line, "\n")] = 0;
    if(sscanf(line, "%255s %255s", command, option) < 1) {
      continue;
    }
    
    if(strcmp(command, "exit") == 0) {
      break;
    }

    bash_pid = fork();
    if(bash_pid == 0) {
      char path[MAX_NAME_LEN + 10];
      snprintf(path, sizeof(path), "/bin/%s", command);
      
      if(strlen(option) > 0) {
        execl(path, command, option, NULL);
      } else {
        execl(path, command, NULL);
      }

      perror("execl failed");
      exit(EXIT_FAILURE);
    } else if(bash_pid > 0) {
      waitpid(bash_pid, &status, 0);
      printf("command exited with status: %d (%s)\n",
            WEXITSTATUS(status), 
            (WEXITSTATUS(status) == 0) ? "success" : "failure");
    } else {
      perror("fork failed");
      exit(EXIT_FAILURE);
    }
    
    option[0] = '\0';
  }
  
  return EXIT_SUCCESS;
}