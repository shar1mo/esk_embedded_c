#include <stdio.h>
#include <stdint.h>
#include "func.h"

int main(void) {
  double_linked_list_t list;
  list_init(&list);
  char choice = 0;
  int16_t index = 0;
  while (1)
  {
    printf(" 1) Добавить абонента\n 2) Удалить абонента\n 3) Поиск абонента по имени\n 4) Вывод всех записей\n 5) Выход\n");
    if (scanf(" %c", &choice) != 1) {
      printf("error input\n");
      return -1;
    }
    switch (choice)
    {
    case '1':
      add_node(&list);
      break;
    case '2':
      delete(&list, list.tail);
      break;
    case '3':
      find_name(&list);
      break;
    case '4':
      print_all(&list);
      break;
    case '5':
      list_free(&list);
      printf("exit\n");
      return 0;
    default:
      printf("error input\n");
      break;
    }
  }
  
  return 0;
}