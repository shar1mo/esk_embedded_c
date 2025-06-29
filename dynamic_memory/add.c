#include "func.h"

int add_node(double_linked_list_t* list) {
    abonent_t new_abonent;
    printf("input name: ");
    scanf("%9s", new_abonent.name);
    printf("input second name: ");
    scanf("%9s", new_abonent.second_name);
    printf("input telephone: ");
    scanf("%11s", new_abonent.telephone);

    node_t* new_node = malloc(sizeof(node_t));
    if(!new_node) {
        printf("error malloc memory\n");
        return -1;
    }

    new_node->data = new_abonent;
    new_node->next = NULL;
    new_node->prev = list->tail;

    if(list->tail) {
        list->tail->next = new_node;
        list->tail = new_node;
    } else {
        list->head = list->tail = new_node;
    }

    list->size++;

    return 0;
}