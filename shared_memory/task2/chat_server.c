#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 0x1234
#define MAX_CLIENTS 10
#define MAX_NAME_LEN 32
#define MAX_TEXT_LEN 256
#define MAX_MESSAGES 100

typedef struct {
    char sender[MAX_NAME_LEN];
    char text[MAX_TEXT_LEN];
} ChatMessage;

typedef struct {
    ChatMessage messages[MAX_MESSAGES];
    int message_count;
    char clients[MAX_CLIENTS][MAX_NAME_LEN];
    int client_count;
    int processed_count;
} ChatRoom;

int main() {
    int shmid;
    ChatRoom *chat_room;
    
    shmid = shmget(SHM_KEY, sizeof(ChatRoom), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    
    chat_room = (ChatRoom*)shmat(shmid, NULL, 0);
    if (chat_room == (ChatRoom*)-1) {
        perror("shmat");
        exit(1);
    }
    
    chat_room->message_count = 0;
    chat_room->client_count = 0;
    chat_room->processed_count = 0;
    
    printf("[SERVER] Chat room created. Waiting for clients...\n");
    
    while (1) {
        for (int i = chat_room->processed_count; i < chat_room->message_count; i++) {
            ChatMessage *msg = &chat_room->messages[i];
            
            if (strncmp(msg->text, "CONNECT:", 8) == 0) {
                if (chat_room->client_count < MAX_CLIENTS) {
                    strcpy(chat_room->clients[chat_room->client_count], msg->sender);
                    chat_room->client_count++;
                    
                    printf("[SERVER] %s connected.\n", msg->sender);
                    
                    if (chat_room->message_count < MAX_MESSAGES) {
                        ChatMessage *sysmsg = &chat_room->messages[chat_room->message_count];
                        strcpy(sysmsg->sender, "SERVER");
                        snprintf(sysmsg->text, MAX_TEXT_LEN, "User %s joined the chat", msg->sender);
                        chat_room->message_count++;
                    }
                    
                    if (chat_room->message_count < MAX_MESSAGES) {
                        ChatMessage *user_msg = &chat_room->messages[chat_room->message_count];
                        strcpy(user_msg->sender, "SERVER");
                        strcpy(user_msg->text, "USERLIST:");
                        for (int j = 0; j < chat_room->client_count; j++) {
                            strcat(user_msg->text, chat_room->clients[j]);
                            if (j < chat_room->client_count - 1) {
                                strcat(user_msg->text, ",");
                            }
                        }
                        chat_room->message_count++;
                    }
                }
            } else if (strncmp(msg->text, "DISCONNECT:", 11) == 0) {
                printf("[SERVER] %s disconnected.\n", msg->sender);
                
                for (int j = 0; j < chat_room->client_count; j++) {
                    if (strcmp(chat_room->clients[j], msg->sender) == 0) {
                        for (int k = j; k < chat_room->client_count - 1; k++) {
                            strcpy(chat_room->clients[k], chat_room->clients[k + 1]);
                        }
                        chat_room->client_count--;
                        
                        if (chat_room->message_count < MAX_MESSAGES) {
                            ChatMessage *sysmsg = &chat_room->messages[chat_room->message_count];
                            strcpy(sysmsg->sender, "SERVER");
                            snprintf(sysmsg->text, MAX_TEXT_LEN, "User %s left the chat", msg->sender);
                            chat_room->message_count++;
                        }
                        
                        if (chat_room->message_count < MAX_MESSAGES) {
                            ChatMessage *user_msg = &chat_room->messages[chat_room->message_count];
                            strcpy(user_msg->sender, "SERVER");
                            strcpy(user_msg->text, "USERLIST:");
                            for (int j = 0; j < chat_room->client_count; j++) {
                                strcat(user_msg->text, chat_room->clients[j]);
                                if (j < chat_room->client_count - 1) {
                                    strcat(user_msg->text, ",");
                                }
                            }
                            chat_room->message_count++;
                        }
                        break;
                    }
                }
            } else if (strcmp(msg->sender, "SERVER") != 0) {
                printf("[%s]: %s\n", msg->sender, msg->text);
            }
            
            chat_room->processed_count++;
        }
        
        sleep(1);
    }
    
    shmdt(chat_room);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}