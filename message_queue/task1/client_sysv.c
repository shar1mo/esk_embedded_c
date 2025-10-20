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

    msgid = msgget(MSG_KEY, 0666);
    if (msgid < 0) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    if (msgrcv(msgid, &msg, MSG_SIZE, 1, 0) < 0) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    printf("[CLIENT] Получено: %s\n", msg.mtext);

    msg.mtype = 2;
    strcpy(msg.mtext, "Hello!");
    if (msgsnd(msgid, &msg, strlen(msg.mtext) + 1, 0) < 0) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("[CLIENT] Отправлено: %s\n", msg.mtext);

    return 0;
}
