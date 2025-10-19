#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#define MAX_LINE_LEN 1024
#define MAX_TOKENS 64
#define PATH_MAX 4096

void print_cwd() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s$ ", cwd);
        fflush(stdout);
    } else {
        perror("getcwd");
        printf("$ ");
        fflush(stdout);
    }
}

int split_tokens(char *s, char *argv[], int max_tokens) {
    int i = 0;
    char *tok = NULL;
    while (i < max_tokens - 1) {
        while (*s && (*s == ' ' || *s == '\t')) s++;
        if (*s == '\0') break;
        tok = s;
        while (*s && *s != ' ' && *s != '\t') s++;
        if (*s) {
            *s = '\0';
            s++;
        }
        argv[i++] = tok;
    }
    argv[i] = NULL;
    return i;
}

int main(void) {
    char line[MAX_LINE_LEN];

    while (1) {
        print_cwd();

        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        char *p = line;
        while (*p && (*p == ' ' || *p == '\t')) p++;
        if (*p == '\0') continue;

        {
            char tmp[MAX_LINE_LEN];
            strncpy(tmp, line, sizeof(tmp));
            tmp[sizeof(tmp)-1] = '\0';
            char *argv_tmp[MAX_TOKENS];
            split_tokens(tmp, argv_tmp, MAX_TOKENS);
            if (argv_tmp[0] && strcmp(argv_tmp[0], "exit") == 0) break;
        }

        char *pipe_pos = strchr(line, '|');

        if (pipe_pos == NULL) {
            char *argv[MAX_TOKENS];
            split_tokens(line, argv, MAX_TOKENS);
            if (argv[0] == NULL) continue;

            pid_t pid = fork();
            if (pid < 0) {
                perror("fork");
                continue;
            } else if (pid == 0) {
                execvp(argv[0], argv);
                fprintf(stderr, "exec failed: %s: %s\n", argv[0], strerror(errno));
                exit(EXIT_FAILURE);
            } else {
                int status;
                waitpid(pid, &status, 0);
                if (WIFEXITED(status)) {
                    printf("command exited with status: %d (%s)\n",
                           WEXITSTATUS(status),
                           (WEXITSTATUS(status) == 0) ? "success" : "failure");
                } else if (WIFSIGNALED(status)) {
                    printf("command killed by signal: %d\n", WTERMSIG(status));
                }
            }
        } else {
            *pipe_pos = '\0';
            char *left = line;
            char *right = pipe_pos + 1;

            while (*left && (*left == ' ' || *left == '\t')) left++;
            char *end;
            end = left + strlen(left) - 1;
            while (end > left && (*end == ' ' || *end == '\t')) { *end = '\0'; end--; }

            while (*right && (*right == ' ' || *right == '\t')) right++;
            end = right + strlen(right) - 1;
            while (end > right && (*end == ' ' || *end == '\t')) { *end = '\0'; end--; }

            if (*left == '\0' || *right == '\0') {
                fprintf(stderr, "syntax error: missing command around '|'\n");
                continue;
            }

            char *argv_left[MAX_TOKENS];
            char *argv_right[MAX_TOKENS];
            split_tokens(left, argv_left, MAX_TOKENS);
            split_tokens(right, argv_right, MAX_TOKENS);

            int pipefd[2];
            if (pipe(pipefd) == -1) {
                perror("pipe");
                continue;
            }

            pid_t pid1 = fork();
            if (pid1 < 0) {
                perror("fork");
                close(pipefd[0]); close(pipefd[1]);
                continue;
            } else if (pid1 == 0) {
                if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(pipefd[0]);
                close(pipefd[1]);

                execvp(argv_left[0], argv_left);
                fprintf(stderr, "exec failed: %s: %s\n", argv_left[0], strerror(errno));
                exit(EXIT_FAILURE);
            }

            pid_t pid2 = fork();
            if (pid2 < 0) {
                perror("fork");
                kill(pid1, SIGKILL);
                close(pipefd[0]); close(pipefd[1]);
                continue;
            } else if (pid2 == 0) {
                if (dup2(pipefd[0], STDIN_FILENO) == -1) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(pipefd[1]);
                close(pipefd[0]);

                execvp(argv_right[0], argv_right);
                fprintf(stderr, "exec failed: %s: %s\n", argv_right[0], strerror(errno));
                exit(EXIT_FAILURE);
            }

            close(pipefd[0]);
            close(pipefd[1]);

            int status;
            waitpid(pid1, &status, 0);
            if (WIFEXITED(status)) {
                printf("[left] exited with status: %d (%s)\n",
                       WEXITSTATUS(status),
                       (WEXITSTATUS(status) == 0) ? "success" : "failure");
            } else if (WIFSIGNALED(status)) {
                printf("[left] killed by signal: %d\n", WTERMSIG(status));
            }

            waitpid(pid2, &status, 0);
            if (WIFEXITED(status)) {
                printf("[right] exited with status: %d (%s)\n",
                       WEXITSTATUS(status),
                       (WEXITSTATUS(status) == 0) ? "success" : "failure");
            } else if (WIFSIGNALED(status)) {
                printf("[right] killed by signal: %d\n", WTERMSIG(status));
            }
        }
    }

    return EXIT_SUCCESS;
}
