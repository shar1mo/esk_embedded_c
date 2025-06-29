#include "func.h"

int find_name(double_linked_list_t* list) {
    int found = 0;
    node_t* search_node = list->head;

    if(!list->head) {
        printf("list is empty\n");
        return -1;
    }

    printf("input search name: ");
    char search_name[NAME_LEN];
    
    if(scanf("%s", search_name) != 1) {
        printf("err input\n");
        return -1;
    }

    while(search_node) {
        if(strcmp(search_node->data.name, search_name) == 0) {
            printf("name: %s\nsecond name: %s\ntelephone: %s\n", search_node->data.name, 
                                                                search_node->data.second_name, 
                                                                search_node->data.telephone);
            putchar('\n');
            found = 1;
        } 
        search_node = search_node->next;
    }
    if(!found) {
        printf("abonent not found\n");
    }
    
    return 0;
}