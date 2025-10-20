#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MSG_KEY 0x1234
#define MSG_SIZE 128

struct msgbuf {
    long mtype;
    char mtext[MSG_SIZE];
};

int main(void) {
    struct msgbuf msg;
    int msgid;

    msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
    if (msgid < 0) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("[SERVER] Очередь создана (key=%x, id=%d)\n", MSG_KEY, msgid);

    msg.mtype = 1;
    strcpy(msg.mtext, "Hi!");
    if (msgsnd(msgid, &msg, strlen(msg.mtext) + 1, 0) < 0) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("[SERVER] Отправлено: %s\n", msg.mtext);

    if (msgrcv(msgid, &msg, MSG_SIZE, 2, 0) < 0) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    printf("[SERVER] Получено: %s\n", msg.mtext);

    msgctl(msgid, IPC_RMID, NULL);
    printf("[SERVER] Очередь удалена\n");

    return 0;
}
