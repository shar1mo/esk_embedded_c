#include <stdio.h>
#include "func.h"

int delete(struct abonent* abonent, int idx) {
  printf("choice abonent to delete(0-%d):\n", MAX_ABONENTS - 1);
  
  if(scanf("%d", &idx) != 1 || idx < 0 || idx >= MAX_ABONENTS) {
    printf("exceeding the user limit\n");
    return 1;
  }
  abonent[idx].name[0] = '\0';
  abonent[idx].second_name[0] = '\0';
  abonent[idx].tel[0] = '\0';
  
  printf("abonent %d deleted\n", idx);
  return 0;
}