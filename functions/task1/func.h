#ifndef FUNC_H
#define FUNC_H

#define MAX_ABONENTS 100
#define NAME_LEN 10
#define TEL_LEN 12

struct abonent
{
  char name[NAME_LEN ];
  char second_name[NAME_LEN];
  char tel[TEL_LEN];
};

int add(struct abonent* abonent, int idx);
int delete(struct abonent* abonent, int idx);
int search(struct abonent* abonent);
void print_all(struct abonent* abonent);

#endif