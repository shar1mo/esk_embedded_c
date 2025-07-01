#include "libcalc.h"

int div(void) {
  int num1 = 0, num2 = 0;

  printf("input num1: ");
  if(scanf("%d", &num1) != 1) {
    printf("error input\n");
    return -1;
  }

  printf("input num2: ");
  if(scanf("%d", &num2) != 1) {
    printf("error input\n");
    return -1;
  }

  printf("result: %d\n", num1 / num2);
  return 0;
}