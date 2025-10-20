#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define SERVER_QUEUE "/chat_server"
#define MAX_CLIENTS 10
#define MAX_NAME_LEN 32
#define MAX_TEXT_LEN 256

typedef struct {
    char sender[MAX_NAME_LEN];
    char text[MAX_TEXT_LEN];
} ChatMessage;

typedef struct {
    char name[MAX_NAME_LEN];
    char queue_name[64];
} ClientInfo;

ClientInfo clients[MAX_CLIENTS];
int client_count = 0;

void broadcast_message(const ChatMessage *msg) {
    for (int i = 0; i < client_count; i++) {
        mqd_t mq = mq_open(clients[i].queue_name, O_WRONLY);
        if (mq != (mqd_t)-1) {
            mq_send(mq, (const char*)msg, sizeof(*msg), 0);
            mq_close(mq);
        }
    }
}

void send_user_list() {
    ChatMessage user_msg;
    strcpy(user_msg.sender, "SERVER");
    strcpy(user_msg.text, "USERLIST:");
    
    for (int i = 0; i < client_count; i++) {
        strcat(user_msg.text, clients[i].name);
        if (i < client_count - 1) {
            strcat(user_msg.text, ",");
        }
    }
    
    broadcast_message(&user_msg);
}

int main(void) {
    mqd_t server_mq;
    struct mq_attr attr;
    ChatMessage msg;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(ChatMessage);
    attr.mq_curmsgs = 0;

    mq_unlink(SERVER_QUEUE);
    server_mq = mq_open(SERVER_QUEUE, O_CREAT | O_RDONLY, 0666, &attr);
    if (server_mq == (mqd_t)-1) {
        perror("mq_open server");
        exit(EXIT_FAILURE);
    }

    printf("[SERVER] Started. Waiting for clients...\n");

    while (1) {
        if (mq_receive(server_mq, (char*)&msg, sizeof(msg), NULL) == -1) {
            perror("mq_receive");
            continue;
        }

        if (strncmp(msg.text, "CONNECT:", 8) == 0) {
            if (client_count >= MAX_CLIENTS) continue;
            strcpy(clients[client_count].name, msg.sender);
            strcpy(clients[client_count].queue_name, msg.text + 8);
            client_count++;

            printf("[SERVER] %s connected.\n", msg.sender);

            ChatMessage sysmsg;
            strcpy(sysmsg.sender, "SERVER");
            snprintf(sysmsg.text, MAX_TEXT_LEN, "User %s joined the chat", msg.sender);
            broadcast_message(&sysmsg);
            
            send_user_list();
        } else if (strncmp(msg.text, "DISCONNECT:", 11) == 0) {
            printf("[SERVER] %s disconnected.\n", msg.sender);
            
            for (int i = 0; i < client_count; i++) {
                if (strcmp(clients[i].name, msg.sender) == 0) {
                    for (int j = i; j < client_count - 1; j++) {
                        clients[j] = clients[j + 1];
                    }
                    client_count--;
                    
                    ChatMessage sysmsg;
                    strcpy(sysmsg.sender, "SERVER");
                    snprintf(sysmsg.text, MAX_TEXT_LEN, "User %s left the chat", msg.sender);
                    broadcast_message(&sysmsg);
                    
                    send_user_list();
                    break;
                }
            }
        } else {
            printf("[%s]: %s\n", msg.sender, msg.text);
            broadcast_message(&msg);
        }
    }

    mq_close(server_mq);
    mq_unlink(SERVER_QUEUE);
    return 0;
}