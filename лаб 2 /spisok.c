#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
  char name[30];
  char vuz[40];
  char city[20];
  char kurs[3];
  char spec[11]; 
  struct student *next;  /* указатель на следующую запись */
  struct student *prior;  /* указатель на предыдущую запись */
};

struct student *start;  /* указатель на первую запись списка */
struct student *last;  /* указатель на последнюю запись */
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
  start = last = NULL;  /* инициализация указателей на начало и конец */

  for(;;) {
    switch(menu_select()) {
      case 1: enter(); /* ввод информации */
        break;
      case 2: mldelete(&start, &last); /* удаление информации */
        break;
      case 3: list(); /* отображение списка */
        break;
      case 4: search(); /* поиск информации */
        break;
      case 5: save();  /* запись списка в файл */
        break;
      case 6: load();  /* считывание с диска */
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

  printf("1. Ввод информации\n");
  printf("2. Удаление информации\n");
  printf("3. Отображение содержимого списка\n");
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

/* Ввод информации . */
void enter(void)
{
  struct student *info;

  for(;;) {
    info = (struct student *)malloc(sizeof(struct student));
    if(!info) {
      printf("\nНет свободной памяти");
      return;
    }

    inputs("Введите имя: ", info->name, 30);
    if(!info->name[0]) break;  /* завершить ввод */
    inputs("Введите вуз: ", info->vuz, 40);
    inputs("Введите город: ", info->city, 20);
    inputs("Введите курс: ", info->kurs, 3);
    inputs("Введите код специальности: ", info->spec, 10);

    dls_store(info, &start, &last);
  } /* цикл ввода */
}

/* Следующая функция вводит с клавиатуры строку
   длинной не больше count и предотвращает переполнение
   строки. Кроме того, она выводит на экран подсказку. */
void inputs(char *prompt, char *s, int count)
{
  char p[255];

  do {
    printf(prompt);
    fgets(p, 254, stdin);
    if(strlen(p) > count) printf("\nСлишком длинная строка\n");
  } while(strlen(p) > count);

  p[strlen(p)-1] = 0; /* удалить символ перевода строки */
  strcpy(s, p);
}

/* Создание упорядоченного двусвязного списка. */
void dls_store(
  struct student *i,   /* новый элемент */
  struct student **start, /* первый элемент списка */
  struct student **last /* последний элемент списка */
)
{
  struct student *old, *p;

  if(*last==NULL) {  /* первый элемент списка */
    i->next = NULL;
    i->prior = NULL;
    *last = i;
    *start = i;
    return;
  }
  p = *start; /* начать с начала списка */

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
      i->next = p; /* новый первый элемент */
      i->prior = NULL;
      p->prior = i;
      *start = i;
      return;
    }
  }
  old->next = i; /* вставка в конец */
  i->next = NULL;
  i->prior = old;
  *last = i;
}

/* Удаление элемента из списка. */
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
    free(info);  /* освободить память */
  }
}

/* Поиск информации. */
struct student *find( char *name)
{
  struct student *info;

  info = start;
  while(info) {
    if(!strcmp(name, info->name)) return info;
    info = info->next;  /* перейти к следующему студенту */
  }
  printf("Имя не найдено.\n");
  return NULL;  /* нет подходящего элемента */
}

/* Отобразить на экране весь список. */
void list(void)
{
  struct student *info;

  info = start;
  while(info) {
    display(info);
    info = info->next;  /* перейти к следующему студенту */
  }
  printf("\n\n");
}

/* Данная функция выполняет собственно вывод на экран
   всех полей записи, содержащей адрес. */
void display(struct student *info)
{
    printf("%s\n", info->name);
    printf("%s\n", info->vuz);
    printf("%s\n", info->city);
    printf("%s\n", info->kurs);
    printf("%s\n", info->spec);
    printf("\n\n");
}

/* Поиск информации в списке. */
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

/* Сохранить список в дисковом файле. */
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
    info = info->next;  /* перейти к следующему студенту */
  }
  fclose(fp);
}

/* Загрузка адресов из файла. */
void load()
{
  struct student *info;
  FILE *fp;

  fp = fopen("mlist", "rb");
  if(!fp) {
    printf("Невозможно открыть файл.\n");
    exit(1);
  }

  /* освободить память, если в памяти уже есть список */
  while(start) {
    info = start->next;
    free(info);
    start = info;
  }

  /* сбросить указатели на начало и конец */
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