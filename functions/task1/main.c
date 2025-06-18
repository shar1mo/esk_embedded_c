#include <stdio.h>
#include "func.h"

int main(void)
{
  struct abonent ab[MAX_ABONENTS] = {0};
  char choice;
  int index = 0;

  while(1)
  {
    printf(" 1) Добавить абонента\n 2) Удалить абонента\n 3) Поиск абонента по имени\n 4) Вывод всех записей\n 5) Выход\n");
    scanf(" %c", &choice);
    switch (choice)
    {
    case '1':
      add(ab, index);
      break;
    case '2':
      delete(ab, index);
      break;
    case '3':
      search(ab);
      break;
    case '4':
      print_all(ab);
      break;
    case '5':
      printf("exit\n");
      return 0;
    default:
      printf("incorrect choice\n");
      break;
    }
  }
}