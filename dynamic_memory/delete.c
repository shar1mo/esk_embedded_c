#include "func.h"

int delete(double_linked_list_t* list, node_t* delete_node) {
    if(!list || !delete_node) {
        printf("list is empty\n");
        return -1;
    }

    if(delete_node->prev) {
        delete_node->prev->next = delete_node->next;
    } else {
        list->head = delete_node->next;
    }

    if(delete_node->next) {
        delete_node->next->prev = delete_node->prev;
    } else {
        list->tail = delete_node->prev;
    }

    free(delete_node);
    list->size--;
    printf("abonent delete\n");
    return 0;
}