#include <stdio.h>

#define BYTE_MASK 0xFF
#define THIRD_BYTE_SHIFT 16

void print_positive_binary(unsigned int num) {
  for (int i = 31; i >= 0; i--) {
      putchar((num & (1u << i)) ? '1' : '0');
      if (i % 8 == 0) putchar(' ');
  }
  putchar('\n');
}

void print_negative_binary(int num){
  unsigned int unum = (unsigned int)num;
  for (int i = 31; i >= 0; i--){
      putchar((unum & (1u << i)) ? '1' : '0');
      if (i % 8 == 0) putchar(' ');
  }
  putchar('\n');
}

int count_ones_in_binary(unsigned int num) {
  int counter = 0;   
  if ((num & 1) == 1){
      counter++;
  }
  if (num > 1){
      counter += count_ones_in_binary(num >> 1);
  }
  return counter;
}

void replace_third_byte(){
  unsigned int num, newByte;

  printf("input positive number for 4th: ");
  scanf("%u", &num);
  printf("original bin number: \n");
  print_positive_binary(num);

  printf("input byte value to replace 3rd byte: ");
  scanf("%u", &newByte);

  if (newByte > 255){
      printf("invalid byte value \n");
      return;
  }

  num &= ~(BYTE_MASK << THIRD_BYTE_SHIFT);
  num |= (newByte << THIRD_BYTE_SHIFT);

  printf("new number in bin: \n");
  print_positive_binary(num);
  printf("new number in dec: %u\n", num);
}

int main() {
  unsigned int num1;
  printf("input positive number for print_positive_binary: ");
  scanf("%u", &num1);
  printf("positive bin: \n");
  print_positive_binary(num1);
  putchar('\n');

  int num2;
  printf("input negative number for print_negative_binary: ");
  scanf("%d", &num2);
  printf("negative bin: \n");
  print_negative_binary(num2);
  putchar('\n');

  unsigned int num3;
  printf("input positive number for 3rd: ");
  scanf("%u", &num3);
  printf("one counter: ");
  int count = count_ones_in_binary(num3);
  printf("%d\n", count);
  putchar('\n');

  replace_third_byte();
  putchar('\n');

  return 0;
}