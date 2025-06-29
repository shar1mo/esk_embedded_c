#include "func.h"

void list_init(double_linked_list_t* list) {
    list->head = list->tail = NULL;
    list->size = 0; 
}

void list_free(double_linked_list_t* list) {
    if(!list) {
        printf("list already empty");
        return;
    }
    node_t* current = list->head;
    while (current)
    {
        node_t* next_node = current->next;
        free(current);
        current = next_node;
    }

    list->head = list->tail = NULL;
    list->size = 0;
}