#include <stdio.h>
#include <string.h>
#include "func.h"

int search(struct abonent* abonent) {
  char name[10] = {0};
  int found = 0;
  printf("input abonents name: \n");
  
  if(scanf("%9s", name) != 1) {
    printf("error input\n");
    return 1;
  }
  
  for (int i = 0; i < MAX_ABONENTS; ++i) {
    if (strcmp(name, abonent[i].name) == 0) {
      printf("abonent №%d name: %9s second name: %9s tel: %11s\n", i, abonent[i].name, abonent[i].second_name, abonent[i].tel);
      found = 1;
      return 0;
    }
  }
  
  if (!found) {
    printf("abonent not found\n");
    return 1;
  }
}