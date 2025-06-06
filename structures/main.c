#include <stdio.h>
#include <string.h>

#define N 100

struct abonent
{
  char name[10];
  char second_name[10];
  char tel[12];
};

void clear_input_buffer()
{
  int c;
  while((c = getchar()) != '\n' && c != EOF);
}

int main(void)
{
  struct abonent ab[N] = {0};
  char choice;
  int index;
  int found;

  while (1)
  {
    printf(" 1) Добавить абонента\n 2) Удалить абонента\n 3) Поиск абонента по имени\n 4) Вывод всех записей\n 5) Выход\n");
    scanf(" %c", &choice);
    clear_input_buffer();
    switch (choice)
    {
    case '1':
      printf("choice free abonent(0-%d): \n", N - 1);
      scanf("%d", &index);
      if(index < 0 || index >= N){
        printf("exceeding the user limit\n");
        break;
      }
      clear_input_buffer();

      if(ab[index].name[0] == '\0' && ab[index].second_name[0] == '\0' && ab[index].tel[0] == '\0')
      {
        printf("fill out the abonent\n");
        printf("name: \n");
        scanf("%9s",ab[index].name);
        clear_input_buffer();

        printf("second name: \n");
        scanf("%9s",ab[index].second_name);
        clear_input_buffer();

        printf("phone number: \n");
        scanf("%11s",ab[index].tel);
        clear_input_buffer();

        printf("new abonent: \n");
        printf("name: %9s second name: %9s telephone %11s\n", ab[index].name, ab[index].second_name, ab[index].tel);
        break;
      }else{
        printf("abonent not free\n");
        break;
      }
    case '2':
      printf("choice abonent to delete(0-%d):\n", N - 1);
      scanf("%d", &index);
      if(index < 0 || index >= N){
        printf("exceeding the user limit\n");
        break;
      }
      clear_input_buffer();

      ab[index].name[0] = '\0';
      ab[index].second_name[0] = '\0';
      ab[index].tel[0] = '\0';
      printf("abonent %d deleted\n", index);
      break;
    case '3':
      char nameab[10] = {0};
      printf("input abonents name: \n");
      scanf("%9s", nameab);
      clear_input_buffer();
      found = 0;
      for(int j = 0; j < N; j++)
      {
        int res = strcmp(nameab, ab[j].name);
        if(res == 0){
          printf("abonent №%d name: %9s second name: %9s tel: %11s\n", j, ab[j].name, ab[j].second_name, ab[j].tel);
          found = 1;
        }
      }
      if(!found){
        printf("abonent not found\n");
      }
      break;
    case '4':
      for(int i = 0; i < N; i++)
      {
        printf("abonent num %d\n", i);
        printf("name: %9s ", ab[i].name);
        printf("second name: %9s ", ab[i].second_name);
        printf("telephone: %11s ", ab[i].tel);
        printf("\n");
      }
      break;
    case '5':
      printf("exit\n");
      return 0;
    default:
      printf("incorrect choice\n");
      break;
    }
  }
  return 0;
}
