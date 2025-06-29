#ifndef FUNC_H
#define FUNC_H

#define NAME_LEN 10
#define TEL_LEN 12


#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>


typedef struct abonent_t
{
  char name[NAME_LEN];
  char second_name[NAME_LEN];
  char telephone[TEL_LEN];
} abonent_t;

typedef struct node_t
{
  struct node_t* next;
  struct node_t* prev;
  abonent_t data;
} node_t;

typedef struct double_linked_list_t
{
  node_t* head;
  node_t* tail;
  size_t size;
} double_linked_list_t;

void list_init(double_linked_list_t* list);
void list_free(double_linked_list_t* list);
int add_node(double_linked_list_t* list);
int delete(double_linked_list_t* list, node_t* delete_node);
int find_name(double_linked_list_t* list);
void print_all(double_linked_list_t* list);

#endif