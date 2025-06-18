#include <stdio.h>
#include "func.h"

int add(struct abonent* abonent, int idx) {
  printf("choice free abonent(0-%d)\n", MAX_ABONENTS - 1);
  if(scanf("%d", &idx) != 1 || idx < 0 || idx >= MAX_ABONENTS) {
    printf("exceeding the user limit\n");
    return 1;
  }

  if(abonent[idx].name[0] == '\0' && abonent[idx].second_name[0] == '\0' && abonent[idx].tel[0] == '\0') {
    printf("fill out the abonent\n");
    printf("enter name (max %d chars): ", NAME_LEN-1);
    if(scanf("%9s",abonent[idx].name) != 1) {
      printf("error input\n");
      return 1;
    }


    printf("\nenter second name (max %d chars): ", NAME_LEN-1);
    if(scanf("%9s",abonent[idx].second_name) != 1) {
      printf("error input\n");
      return 1;
    }


    printf("\nenter phone (max %d digits): ", TEL_LEN-1);
    if(scanf("%11s",abonent[idx].tel) != 1) {
      printf("error input\n");
      return 1;
    }


    printf("\nnew abonent: \n");
    printf("name: %9s second name: %9s telephone %11s\n", abonent[idx].name, abonent[idx].second_name, abonent[idx].tel);
    return 0;
    } else {
      printf("abonent not free\n");
      return 1;
    }
}