#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define SERVER_TO_CLIENT "/mq_server_to_client"
#define CLIENT_TO_SERVER "/mq_client_to_server"
#define MAX_SIZE 128

int main(void) {
    mqd_t mq_to_server, mq_from_server;
    char buffer[MAX_SIZE];

    mq_from_server = mq_open(SERVER_TO_CLIENT, O_RDONLY);
    mq_to_server   = mq_open(CLIENT_TO_SERVER, O_WRONLY);
    if (mq_to_server == (mqd_t)-1 || mq_from_server == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    if (mq_receive(mq_from_server, buffer, MAX_SIZE, NULL) == -1) {
        perror("mq_receive");
        exit(EXIT_FAILURE);
    }
    printf("[CLIENT] Получено от сервера: %s\n", buffer);

    strcpy(buffer, "Hello!");
    if (mq_send(mq_to_server, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }
    printf("[CLIENT] Отправлено серверу: %s\n", buffer);

    mq_close(mq_to_server);
    mq_close(mq_from_server);
    return 0;
}
