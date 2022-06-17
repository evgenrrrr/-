#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

// Структура описывает строку файла
struct Row {
	char first_name[50];
	char last_name[50];
	char gender[10];
	char occupation[50];
	int salary;
};

// Функция проверяет, подходит ли текущая строка под категорию (если категория пустая, то подходит)
int inCategory(struct Row elem, char *gender, char *occupation) {
	if(strcmp(gender, "")) // Категория "Пол" не пустая
		if(strcmp(elem.gender, gender)) // Пол не совпадает
			return 0;
	if(strcmp(occupation, "")) // Категория "Профессия" не пустая
		if(strcmp(elem.occupation, occupation)) // Профессия не совпадает
			return 0;
	return 1;
}

// Функция печатает список с учетом фильтра (категории)
void print(struct Row *list, int n, char *gender, char *occupation) {
	int i;
	for(i = 0; i < n; i = i + 1) {
		if(inCategory(list[i], gender, occupation) == 0) continue;
		printf("%10s | %10s | %10s | %20s | %7d\n", list[i].first_name, list[i].last_name, list[i].gender, list[i].occupation, list[i].salary);
	}
}

// Сравнение по имени для сортировки
int compareFirstName(const void *a, const void *b)
{
  return strcmp(((struct Row*)a)->first_name, ((struct Row*)b)->first_name);
}

// Сравнение по фамилии для сортировки
int compareLastName(const void *a, const void *b)
{
  return strcmp(((struct Row*)a)->last_name, ((struct Row*)b)->last_name);
}

// Сравнение по полу для сортировки
int compareGender(const void *a, const void *b)
{
  return strcmp(((struct Row*)a)->gender, ((struct Row*)b)->gender);
}

// Сравнение по профессии для сортировки
int compareOccupation(const void *a, const void *b)
{
  return strcmp(((struct Row*)a)->occupation, ((struct Row*)b)->occupation);
}

// Сравнение по зарплате для сортировки
int compareSalary(const void *a, const void *b)
{
  return ((struct Row*)a)->salary - ((struct Row*)b)->salary;
}

// Сравнение по зарплате по убыванию для сортировки для гистограммы
int compareSalaryDesc(const void *a, const void *b)
{
  return ((struct Row*)b)->salary - ((struct Row*)a)->salary;
}

// Сортировка по колонке
void orderBy(struct Row *list, int n, int k, int desc) {
	switch(k) {
		case 1:
			qsort(list, n, sizeof(struct Row), compareFirstName);
			break;
		case 2:
			qsort(list, n, sizeof(struct Row), compareLastName);
			break;
		case 3:
			qsort(list, n, sizeof(struct Row), compareGender);
			break;
		case 4:
			qsort(list, n, sizeof(struct Row), compareOccupation);
			break;
		case 5:
			if(desc) {
				qsort(list, n, sizeof(struct Row), compareSalaryDesc);
			} else {
				qsort(list, n, sizeof(struct Row), compareSalary);
			}
			break;
	}
}

// Функция запрашивает номер колонки и сортирует по нему
int orderByColumn(struct Row *list, int n) {
	char ans[100];
	int a;
	printf("Enter a column number (1 - 5): ");
	scanf("%s", ans);
	if(strlen(ans) > 1) return 0;
	if(!isdigit(ans[0])) return 0;
	a = atoi(ans);
	if(a < 1 || a > 5) return 0;
	orderBy(list, n, a, 0);
	return 1;
}

// Функция запрашивает пол и если он введен корректно, запоминает в категорию
void getGender(struct Row *list, int n, char *gender) {
	char ans[100];
	int i;
	printf("Enter gender: ");
	scanf("%s", ans);
	for(i = 0; i < n; i = i + 1) {
		if(!strcmp(list[i].gender, ans)) {
			strcpy(gender, ans);
			return;
		}
	}
	printf("Gender not found\n");
}

// Функция запрашивает профессию и если она введена корректно, запоминает в категорию
void getOccupation(struct Row *list, int n, char *occupation) {
	char ans[100];
	int i;
	printf("Enter occupation: ");
	scanf("%s", ans);
	for(i = 0; i < n; i = i + 1) {
		if(!strcmp(list[i].occupation, ans)) {
			strcpy(occupation, ans);
			return;
		}
	}
	printf("Occupation not found\n");
}

// Функция очищает категорию
void clearFilter(char *gender, char *occupation) {
	strcpy(gender, "");
	strcpy(occupation, "");
}

// Функция находит медианную зарплату с учетом категории
void getMedianSalary(struct Row *list, int n, char *gender, char *occupation) {
	int i, k = 0, t, odd;
	int a, b;
	float median;
	for(i = 0; i < n; i = i + 1) {
		if(inCategory(list[i], gender, occupation)) k = k + 1;
	}
	t = k / 2;
	odd = k % 2;
	k = 0;
	for(i = 0; i < n; i = i + 1) {
		if(inCategory(list[i], gender, occupation)) k = k + 1;
		if(k == t) a = list[i].salary;
		if(k == t+1) {
			b = list[i].salary;
			break;
		}
	}
	if(odd) median = b;
	else median = (a+b) / 2.0;
	printf("Median salary: %.1f\n", median);
}

// Функция строит гистограмму по зарплате с учетом категории
void buildHistogram(struct Row *list, int n, char *gender, char *occupation) {
	int i, j, k, max = 0;
	char name[100];
	// Сортируем по убыванию зарплаты
	orderBy(list, n, 5, 1);
	// Ищем максимум зарплаты в категории
	for(i = 0; i < n; i = i + 1) {
		if(inCategory(list[i], gender, occupation)) {
			if(list[i].salary > max) max = list[i].salary;
		}
	}
	// Выводим строки гистограммы
	for(i = 0; i < n; i = i + 1) {
		if(inCategory(list[i], gender, occupation)) {
			k = 30.0 * list[i].salary / max;
			strcpy(name, list[i].first_name);
			strcat(name, " ");
			strcat(name, list[i].last_name);
			printf("%30s ", name);
			for(j = 0; j < k; j = j + 1) {
				printf("*");
			}
			printf(" %d\n", list[i].salary);
		}
	}
}

// Функция ищет зарплату по номеру дециля
void getSalaryByDecil(struct Row *list, int n) {
	char ans[100];
	int a, i;
	printf("Enter a decil number (1 - 10): ");
	scanf("%s", ans);
	if(strlen(ans) > 2) return;
	for(i = 0; i < strlen(ans); i = i + 1)
		if(!isdigit(ans[i]))
			return;
	a = atoi(ans);
	if(a < 1 || a > 10) return;
	orderBy(list, n, 5, 0);
	i = a * n / 10 - 1;
	printf("Salary = %d\n", list[i].salary);
}

// Функция выводит меню и обрабатывает выбор пользователя
int readAndExecute(struct Row *list, int n, char *gender, char *occupation) {
	char ans[100];
	int a;
	printf("\nCategory: %s %s\n", gender, occupation);
	printf("0 - Print\n");
	printf("1 - Enter gender\n");
	printf("2 - Enter occupation\n");
	printf("3 - Clear filter\n");
	printf("4 - Find median salary\n");
	printf("5 - Find salary by decil\n");
	printf("6 - Built histogram\n");
	printf("7 - Order by column\n");
	printf("q - Exit\n");
	scanf("%s", ans);
	if(!strcmp(ans, "q")) return 0;
	if(strlen(ans) > 1) return 1;
	if(!isdigit(ans[0])) return 1;
	a = atoi(ans);
	system("cls");
	switch(a) {
		case 0:
			print(list, n, gender, occupation);
			break;
		case 1:
			getGender(list, n, gender);
			break;
		case 2:
			getOccupation(list, n, occupation);
			break;
		case 3:
			clearFilter(gender, occupation);
			break;
		case 4:
			getMedianSalary(list, n, gender, occupation);
			break;
		case 5:
			getSalaryByDecil(list, n);
			break;
		case 6:
			buildHistogram(list, n, gender, occupation);
			break;
		case 7:
			if(orderByColumn(list, n)) print(list, n, gender, occupation);
			break;
	}
	return 1;
}

int main(int argc, char *argv[]) {
	struct Row  *list, *plist;
	FILE *f;
	char s[256];
	char *p;
	char ans[100], gender[100], occupation[100];
	int i, j, n;
	// Запрашиваем имя файла и пытаемся его открыть
	printf("Enter file name: ");
	scanf("%s", ans);
	f = fopen(ans, "r");
	if(f == NULL) {
		printf("File not found!");
		return 0;
	}
	// Считаем количество строк в файле
	n = 0;
	i = -1;
    while((fgets(s, 256, f)) != NULL) n = n + 1;
    n = n - 1; // Заголовок не считаем
    list = (struct Row*)calloc(n, sizeof(struct Row)); // Выделяем память
    fseek(f, 0, SEEK_SET); // Перемещаемся в начало файла
    // Получаем данные
	while((fgets(s, 256, f)) != NULL)
    {
    	j = 0;
    	if(i >= 0) {
    		// Делим строку по символу ; и переводу строки
	    	p = strtok(s, ";\n");
	    	while(p != NULL) {
	    		// В зависимости от номера поля копируем в соответствующее поле структуры
	    		switch(j) {
	    			case 0:
	    				strncpy(list[i].first_name, p, 50);
	    				break;
	    			case 1:
	    				strncpy(list[i].last_name, p, 50);
	    				break;
	    			case 2:
	    				strncpy(list[i].gender, p, 10);
	    				break;
	    			case 3:
	    				strncpy(list[i].occupation, p, 50);
	    				break;
	    			case 4:
	    				list[i].salary = atoi(p);
	    				break;
		    	}
	 	   		p = strtok(NULL, ";\n");
	 	   		j = j + 1;
			}
		}
		i = i + 1;
    }
    
	fclose(f); // Прочитали - закрываем
	
	printf("The file has been read successfully\n");

	strcpy(gender, ""); // Категория пустая
	strcpy(occupation, "");
	while(readAndExecute(list, n, gender, occupation)); // Цикл отрисовки меню
	
	free(list); // Освобождаем память
}