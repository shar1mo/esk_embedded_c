#include "func.h"

void print_all(double_linked_list_t* list) {
    if(!list->head) {
        printf("List is empty\n");
        return;
    }
    
    node_t* curret = list->head;
    while(curret) {
        printf("name: %s\nsecond_name: %s\ntel: %s\n", curret->data.name, 
                                                        curret->data.second_name, 
                                                        curret->data.telephone);
        putchar('\n');
        curret = curret->next;
    }
}