#include "libcalc.h"

int main(void) {
  char choice = 0;
  while(1) {
    printf("1) Сложение\n2) Вычитание\n3) Умножение\n4) Деление\n5) Выход\n");
    if(scanf(" %c", &choice) != 1) {
      printf("error input\n");
      return -1;
    }
    switch (choice)
    {
    case '1':
      add();
      break;
    case '2':
      sub();
      break;
    case '3':
      mul();
      break;
    case '4':
      div();
      break;
    case '5':
      printf("exit\n");
      return 0;
    default:
      printf("error input\n");
      break;
    }
  }
}