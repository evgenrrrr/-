#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
  char name[40];
  char vuz[40];
  char kurs[5];
  char spec[15]; 
  struct student *next;  
  struct student *prior; 
};

struct student *start;  
struct student *last;  
struct student *find(char *);

void enter(void), search(void), save(void);
void load(void), list(void);
void mldelete(struct student **, struct student **);
void dls_store(struct student *i, struct student **start,
               struct student **last);
void inputs(char *, char *, int), display(struct student *);
int menu_select(void);

int main(void)
{
  start = last = NULL;  

  for(;;) {
    switch(menu_select()) {
      case 1: enter(); 
        break;
      case 2: mldelete(&start, &last); 
        break;
      case 3: list(); 
        break;
      case 4: search(); 
        break;
      case 5: save();  
        break;
      case 6: load();  
        break;
      case 7: exit(0);
    }
  }
  return 0;
}

/* Выбор действия пользователя. */
int menu_select(void)
{
  char s[80];
  int c;

  printf("1. Ввести информацию\n");
  printf("2. Удалить информацию\n");
  printf("3. Показать содержимое списка\n");
  printf("4. Поиск\n");
  printf("5. Сохранить в файл\n");
  printf("6. Загрузить из файла\n");
  printf("7. Выход\n");
  do {
    printf("\nВаш выбор: ");
    gets(s);
    c = atoi(s);
  } while(c<0 || c>7);
  return c;
}


void enter(void)
{
  struct student *info;

  for(;;) {
    info = (struct student *)malloc(sizeof(struct student));
    if(!info) {
      printf("\nНет свободной памяти");
      return;
    }

    inputs("Введите имя: ", info->name, 40);
    if(!info->name[0]) break;  /* завершить ввод */
    inputs("Введите вуз: ", info->vuz, 40);
    inputs("Введите курс: ", info->kurs, 5);
    inputs("Введите код специальности: ", info->spec, 15);

    dls_store(info, &start, &last);
  } 
}


void inputs(char *prompt, char *s, int count)
{
  char p[255];

  do {
    printf(prompt);
    fgets(p, 254, stdin);
    if(strlen(p) > count) printf("\nСлишком длинная строка\n");
  } while(strlen(p) > count);

  p[strlen(p)-1] = 0; 
  strcpy(s, p);
}


void dls_store(
  struct student *i,  
  struct student **start, 
  struct student **last 
)
{
  struct student *old, *p;

  if(*last==NULL) { 
    i->next = NULL;
    i->prior = NULL;
    *last = i;
    *start = i;
    return;
  }
  p = *start; 

  old = NULL;
  while(p) {
    if(strcmp(p->name, i->name)<0){
      old = p;
      p = p->next;
    }
    else {
      if(p->prior) {
        p->prior->next = i;
        i->next = p;
        i->prior = p->prior;
        p->prior = i;
        return;
      }
      i->next = p;
      i->prior = NULL;
      p->prior = i;
      *start = i;
      return;
    }
  }
  old->next = i; 
  i->next = NULL;
  i->prior = old;
  *last = i;
}


void mldelete(struct student **start, struct student **last)
{
  struct student *info;
  char s[80];

  inputs("Введите имя: ", s, 30);
  info = find(s);
  if(info) {
    if(*start==info) {
      *start=info->next;
      if(*start) (*start)->prior = NULL;
      else *last = NULL;
    }
    else {
      info->prior->next = info->next;
      if(info!=*last)
          info->next->prior = info->prior;
      else
        *last = info->prior;
    }
    free(info);  
  }
}


struct student *find( char *name)
{
  struct student *info;

  info = start;
  while(info) {
    if(!strcmp(name, info->name)) return info;
    info = info->next; 
  }
  printf("Имя не найдено.\n");
  return NULL; 
}


void list(void)
{
  struct student *info;

  info = start;
  while(info) {
    display(info);
    info = info->next;  
  }
  printf("\n\n");
}


void display(struct student *info)
{
    printf("%s\n", info->name);
    printf("%s\n", info->vuz);
    printf("%s\n", info->kurs);
    printf("%s\n", info->spec);
    printf("\n\n");
}


void search(void)
{
  char name[40];
  struct student *info;

  printf("Введите имя: ");
  gets(name);
  info = find(name);
  if(!info) printf("Не найдено\n");
  else display(info);
}


void save(void)
{
  struct student *info;

  FILE *fp;

  fp = fopen("mlist", "wb");
  if(!fp) {
    printf("Невозможно открыть файл.\n");
    exit(1);
  }
  printf("\nСохранение в файл\n");

  info = start;
  while(info) {
    fwrite(info, sizeof(struct student), 1, fp);
    info = info->next; 
  }
  fclose(fp);
}


void load()
{
  struct student *info;
  FILE *fp;

  fp = fopen("mlist", "rb");
  if(!fp) {
    printf("Невозможно открыть файл.\n");
  
  }

 
  while(start) {
    info = start->next;
    free(info);
    start = info;
  }

  
  start = last = NULL;

  printf("\nЗагрузка из файла\n");
  while(!feof(fp)) {
    info = (struct student *) malloc(sizeof(struct student));
    if(!info) {
      printf("Нет свободной памяти");
      return;
    }
    if(1 != fread(info, sizeof(struct student), 1, fp)) break;
    dls_store(info, &start, &last);
  }
  fclose(fp);
}