#include <stdio.h>

#define BYTE_MASK 0xFF
#define THIRD_BYTE_SHIFT 16

void first(unsigned int num);
void second(int num);
int third(unsigned int num);
void fourth();

int main(){
    unsigned int num1;
    printf("input positive number for first: ");
    scanf("%u", &num1);
    printf("positive bin: \n");
    first(num1);
    putchar('\n');

    int num2;
    printf("input negative number for second: ");
    scanf("%d", &num2);
    printf("negative bin: \n");
    second(num2);
    putchar('\n');

    unsigned int num3;
    printf("input positive number for 3rd: ");
    scanf("%u", &num3);
    printf("one counter: ");
    int count = third(num3);
    printf("%d\n", count);

    fourth();

    return 0;
}

void first(unsigned int num) {
    for (int i = 31; i >= 0; i--) {
        putchar((num & (1u << i)) ? '1' : '0');
        if (i % 8 == 0) putchar(' ');
    }
    putchar('\n');
}

void second(int num){
    unsigned int unum = (unsigned int)num;

    for (int i = 31; i >= 0; i--){
        putchar((unum & (1u << i)) ? '1' : '0');
        if (i % 8 == 0) putchar(' ');
    }
    putchar('\n');
}

int third(unsigned int num){
    int counter = 0;
    
    if ((num & 1) == 1){
        counter++;
    }

    if (num > 1){
        counter += third(num >> 1);
    }

    return counter;
}

void fourth(){
    unsigned int num, newByte;

    printf("input positive number for 4th: ");
    scanf("%u", &num);
    printf("original bin number: \n");
    first(num);

    printf("input byte value to replace 3rd byte: ");
    scanf("%u", &newByte);

    if (newByte > 255){
        printf("invalid byte value \n");
        return;
    }

    num &= ~(BYTE_MASK << THIRD_BYTE_SHIFT);
    num |= (newByte << THIRD_BYTE_SHIFT);

    printf("new number in bin: \n");
    first(num);
    printf("new number in dec: %u\n", num);
}
