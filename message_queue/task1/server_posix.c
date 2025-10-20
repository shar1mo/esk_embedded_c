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
    mqd_t mq_to_client, mq_from_client;
    struct mq_attr attr;
    char buffer[MAX_SIZE];

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mq_unlink(SERVER_TO_CLIENT);
    mq_unlink(CLIENT_TO_SERVER);

    mq_to_client = mq_open(SERVER_TO_CLIENT, O_CREAT | O_WRONLY, 0666, &attr);
    mq_from_client = mq_open(CLIENT_TO_SERVER, O_CREAT | O_RDONLY, 0666, &attr);
    if (mq_to_client == (mqd_t)-1 || mq_from_client == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    printf("[SERVER] Очереди созданы.\n");

    strcpy(buffer, "Hi!");
    if (mq_send(mq_to_client, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }
    printf("[SERVER] Отправлено клиенту: %s\n", buffer);

    if (mq_receive(mq_from_client, buffer, MAX_SIZE, NULL) == -1) {
        perror("mq_receive");
        exit(EXIT_FAILURE);
    }
    printf("[SERVER] Получено от клиента: %s\n", buffer);

    mq_close(mq_to_client);
    mq_close(mq_from_client);
    mq_unlink(SERVER_TO_CLIENT);
    mq_unlink(CLIENT_TO_SERVER);

    printf("[SERVER] Очереди удалены\n");
    return 0;
}
