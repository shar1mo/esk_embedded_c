#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ncurses.h>

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

ChatRoom *chat_room;
char username[MAX_NAME_LEN];
WINDOW *msg_win, *input_win, *users_win;

void update_user_list(const char *users) {
    werase(users_win);
    box(users_win, 0, 0);
    mvwprintw(users_win, 0, 2, " Users ");
    mvwprintw(users_win, 1, 2, "---------------");
    
    char users_copy[MAX_TEXT_LEN];
    strcpy(users_copy, users);
    
    char *token = strtok(users_copy, ",");
    int line = 2;
    
    while (token != NULL && line < getmaxy(users_win) - 1) {
        mvwprintw(users_win, line, 2, "%s", token);
        line++;
        token = strtok(NULL, ",");
    }
    
    wrefresh(users_win);
}

void *receiver_thread(void *arg) {
    int last_message_count = 0;
    
    while (1) {
        if (chat_room->message_count > last_message_count) {
            for (int i = last_message_count; i < chat_room->message_count; i++) {
                ChatMessage *msg = &chat_room->messages[i];
                
                if (strncmp(msg->text, "USERLIST:", 9) == 0) {
                    update_user_list(msg->text + 9);
                } else {
                    werase(input_win);
                    box(input_win, 0, 0);
                    mvwprintw(input_win, 1, 2, "Message: ");
                    wrefresh(input_win);
                    
                    int y, x;
                    getyx(msg_win, y, x);
                    
                    if (y >= getmaxy(msg_win) - 2) {
                        scroll(msg_win);
                        y = getmaxy(msg_win) - 2;
                    }
                    
                    wmove(msg_win, y, 1);
                    
                    char temp_text[MAX_TEXT_LEN];
                    strncpy(temp_text, msg->text, MAX_TEXT_LEN - 1);
                    temp_text[MAX_TEXT_LEN - 1] = '\0';
                    
                    int max_width = getmaxx(msg_win) - 4;
                    char *token = strtok(temp_text, "\n");
                    
                    while (token != NULL) {
                        int len = strlen(token);
                        int pos = 0;
                        
                        while (pos < len) {
                            int chunk = (len - pos > max_width) ? max_width : (len - pos);
                            
                            wprintw(msg_win, "[%s]: ", msg->sender);
                            waddnstr(msg_win, token + pos, chunk);
                            wprintw(msg_win, "\n");
                            
                            pos += chunk;
                            
                            getyx(msg_win, y, x);
                            if (y >= getmaxy(msg_win) - 2) {
                                scroll(msg_win);
                                y = getmaxy(msg_win) - 2;
                            }
                            wmove(msg_win, y, 1);
                            
                            if (pos < len) {
                                wprintw(msg_win, "    ");
                            }
                        }
                        
                        token = strtok(NULL, "\n");
                    }
                    
                    wrefresh(msg_win);
                }
                
                wmove(input_win, 1, 11);
                wrefresh(input_win);
            }
            last_message_count = chat_room->message_count;
        }
        
        usleep(100000);
    }
    return NULL;
}

int main() {
    printf("Enter your name: ");
    scanf("%s", username);
    
    int shmid = shmget(SHM_KEY, sizeof(ChatRoom), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    
    chat_room = (ChatRoom*)shmat(shmid, NULL, 0);
    if (chat_room == (ChatRoom*)-1) {
        perror("shmat");
        exit(1);
    }
    
    if (chat_room->message_count < MAX_MESSAGES) {
        ChatMessage *msg = &chat_room->messages[chat_room->message_count];
        strcpy(msg->sender, username);
        snprintf(msg->text, MAX_TEXT_LEN, "CONNECT:%s", username);
        chat_room->message_count++;
    }
    
    sleep(1);
    
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    
    clear();
    refresh();
    
    int height = LINES - 4;
    int msg_width = COLS - 20;
    
    msg_win = newwin(height, msg_width, 0, 0);
    box(msg_win, 0, 0);
    mvwprintw(msg_win, 0, 2, " Messages ");
    scrollok(msg_win, TRUE);
    wrefresh(msg_win);
    
    users_win = newwin(height, 18, 0, msg_width);
    box(users_win, 0, 0);
    mvwprintw(users_win, 0, 2, " Users ");
    mvwprintw(users_win, 1, 2, "---------------");
    wrefresh(users_win);
    
    input_win = newwin(3, COLS, height, 0);
    box(input_win, 0, 0);
    mvwprintw(input_win, 0, 2, " Input ");
    mvwprintw(input_win, 1, 2, "Message: ");
    wrefresh(input_win);
    
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receiver_thread, NULL);
    
    char input[MAX_TEXT_LEN];
    int input_pos = 0;
    
    while (1) {
        wmove(input_win, 1, 11 + input_pos);
        wrefresh(input_win);
        
        int ch = wgetch(input_win);
        
        if (ch == '\n') {
            if (input_pos > 0) {
                input[input_pos] = '\0';
                
                if (strcmp(input, "/exit") == 0) break;
                
                if (chat_room->message_count < MAX_MESSAGES) {
                    ChatMessage *msg = &chat_room->messages[chat_room->message_count];
                    strcpy(msg->sender, username);
                    strcpy(msg->text, input);
                    chat_room->message_count++;
                }
                
                input_pos = 0;
                mvwprintw(input_win, 1, 11, "                                        ");
                mvwprintw(input_win, 1, 11, "");
                wrefresh(input_win);
            }
        } else if (ch == 127 || ch == KEY_BACKSPACE) {
            if (input_pos > 0) {
                input_pos--;
                mvwaddch(input_win, 1, 11 + input_pos, ' ');
                wmove(input_win, 1, 11 + input_pos);
                wrefresh(input_win);
            }
        } else if (ch >= 32 && ch <= 126 && input_pos < MAX_TEXT_LEN - 1) {
            input[input_pos++] = ch;
            waddch(input_win, ch);
            wrefresh(input_win);
        }
    }

    if (chat_room->message_count < MAX_MESSAGES) {
        ChatMessage *msg = &chat_room->messages[chat_room->message_count];
        strcpy(msg->sender, username);
        strcpy(msg->text, "DISCONNECT:");
        chat_room->message_count++;
    }
    
    sleep(1);
    
    endwin();
    shmdt(chat_room);
    
    return 0;
}