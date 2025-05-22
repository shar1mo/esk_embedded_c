#include <stdio.h>
#include <string.h>

void third_byte_pointer(int *num, unsigned char replace_num);
void arr_pointer(void);
char* search(char line[], char subline[]);

int main()
{ 
    int num_for_third;
    int replace_num_input;
    printf("Input number for third byte replace: ");
    scanf("%d", &num_for_third);
    printf("Input number for replace (0-255): ");
    scanf("%d", &replace_num_input);

    if(replace_num_input < 0 || replace_num_input > 255)
    {
        printf("Error: Replace number must be 0-255\n");
        return 1;
    }
    unsigned char replace_num = (unsigned char)replace_num_input;

    printf("Before: 0x%x (%d)\n", num_for_third, num_for_third);
    third_byte_pointer(&num_for_third, replace_num);
    printf("After:  0x%x (%d)\n\n", num_for_third, num_for_third);

    float x = 5.0;
    printf("x = %f, ", x);
    float y = 6.0;
    printf("y = %f\n", y);
    float *xp = &y;
    float *yp = &y;
    printf("Result: %f\n\n", *xp + *yp);

    arr_pointer();

    char line[100], subline[100];
    printf("\nEnter string: ");
    scanf(" %[^\n]", line);
    printf("Enter substring to search: ");
    scanf(" %[^\n]", subline);
    
    char *result = search(line, subline);
    if(result)
        printf("Substring found at position: %ld\n", result - line);
    else
        printf("Substring not found\n");

    return 0;
}

void third_byte_pointer(int *num, unsigned char replace_num)
{
    unsigned char *byte_ptr = (unsigned char *)num;
    byte_ptr[2] = replace_num;
}

void arr_pointer(void)
{
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int *arr_ptr = arr;
    for(int i = 0; i < 10; i++)
    {
        printf("%2d", *arr_ptr);
        arr_ptr++;
    }
    putchar('\n');
}

char* search(char line[], char subline[]) {
    for(int i = 0; line[i]; i++) {
        int j = 0;
        while(subline[j] && line[i+j] == subline[j]) j++;
        if(!subline[j]) return line + i;
    }
    return NULL;
}