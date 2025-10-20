#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ncurses.h>

#define SERVER_QUEUE "/chat_server"
#define MAX_NAME_LEN 32
#define MAX_TEXT_LEN 256

typedef struct {
    char sender[MAX_NAME_LEN];
    char text[MAX_TEXT_LEN];
} ChatMessage;

char username[MAX_NAME_LEN];
char client_queue_name[64];
mqd_t server_mq, client_mq;

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
    ChatMessage msg;
    while (1) {
        if (mq_receive(client_mq, (char*)&msg, sizeof(msg), NULL) > 0) {
            if (strncmp(msg.text, "USERLIST:", 9) == 0) {
                update_user_list(msg.text + 9);
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
                strncpy(temp_text, msg.text, MAX_TEXT_LEN - 1);
                temp_text[MAX_TEXT_LEN - 1] = '\0';
                
                int max_width = getmaxx(msg_win) - 3;
                char *token = strtok(temp_text, "\n");
                
                while (token != NULL) {
                    int len = strlen(token);
                    int pos = 0;
                    
                    while (pos < len) {
                        int chunk = (len - pos > max_width) ? max_width : (len - pos);
                        
                        wprintw(msg_win, "[%s]: ", msg.sender);
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
                            wprintw(msg_win, "      ");
                        }
                    }
                    
                    token = strtok(NULL, "\n");
                }
                
                wrefresh(msg_win);
            }
            wmove(input_win, 1, 11);
            wrefresh(input_win);
        }
    }
    return NULL;
}

int main(void) {
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(ChatMessage);
    attr.mq_curmsgs = 0;

    printf("Enter your name: ");
    scanf("%s", username);

    snprintf(client_queue_name, sizeof(client_queue_name), "/client_%s", username);

    mq_unlink(client_queue_name);
    client_mq = mq_open(client_queue_name, O_CREAT | O_RDONLY, 0666, &attr);
    server_mq = mq_open(SERVER_QUEUE, O_WRONLY);
    if (server_mq == (mqd_t)-1 || client_mq == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    ChatMessage msg;
    strcpy(msg.sender, username);
    snprintf(msg.text, sizeof(msg.text), "CONNECT:%s", client_queue_name);
    mq_send(server_mq, (const char*)&msg, sizeof(msg), 0);

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
    mvwprintw(users_win, 2, 2, "%s", username);
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
                
                strcpy(msg.sender, username);
                strcpy(msg.text, input);
                mq_send(server_mq, (const char*)&msg, sizeof(msg), 0);
                
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

    strcpy(msg.sender, username);
    strcpy(msg.text, "DISCONNECT:");
    mq_send(server_mq, (const char*)&msg, sizeof(msg), 0);

    endwin();
    mq_close(client_mq);
    mq_close(server_mq);
    mq_unlink(client_queue_name);
    return 0;
}