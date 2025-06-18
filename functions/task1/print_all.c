#include <stdio.h>
#include "func.h"

void print_all(struct abonent* abonent) {
  for(int i = 0; i < MAX_ABONENTS; i++) {
    printf("abonent num %d\n", i);
    printf("name: %9s ", abonent[i].name);
    printf("second name: %9s ", abonent[i].second_name);
    printf("telephone: %11s ", abonent[i].tel);
    printf("\n");
  }
}