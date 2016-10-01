#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

/*
 * FIXIT: из названия переменной NUM непонятно, зачем она нужна. Переименуйте.
 */
#define NUM 100

void Split(char* string, char* delimiters, char*** tokens, int* tokensCount)
{
	char* temp = NULL;
	temp = strtok(string, delimiters);
	if (temp == NULL) 
	{
		printf("error");
		return;
	}
	while (temp != NULL)
	{
		strcpy((*tokens)[*tokensCount], temp);
		(*tokensCount)++;
		temp = strtok(NULL, delimiters);
	}
		(*tokens)[*tokensCount] = NULL;
}

int main(int argc, char const *argv[])
{
	int i = 0;
/*
 * FIXIT: нужно вынести 128 в отдельную константу
 */
	char buffer[128];
  	FILE *file;
    
  /*
   * Очень похожий немного непонятный мне код видел у Дьячкова: зачем в коде программы для запуска задач генерировать файл
   * с списком задач для запуска?
   * Если хотите продемонстрировать пример работы, то просто закоммитьте файл commands.txt
   */
  	file = fopen("commands.txt", "w+t");
	fprintf(file, "3\n");
	fprintf(file, "7 ls -l\n");
	fprintf(file, "3 pwd\n");
	fprintf(file, "1 echo Hello, World\n");
	fclose(file);
	file = fopen("commands.txt", "r");
		
	char** tokens = (char**)malloc(sizeof(char*) * NUM);

	for (i = 0; i < NUM; i++)
	{
		tokens[i] = (char*)malloc(NUM * sizeof(char));
	}
	int tokensCount = 0;
	char delimiters[2] = {' ', '\n'};
	fgets(buffer, 128, file);
	int count = atoi(buffer);
	for (i = 0; i < count; i++)
	{

		fgets(buffer, 128, file);
		printf("%d - %s\n", i, buffer);

		pid_t pid = fork();
		if (pid == 0)
		{
			Split(buffer, delimiters, &tokens, &tokensCount);
			sleep(atoi(tokens[0]));
			printf("\n");
			execvp(tokens[1], tokens + 1);
			exit(0);
		}
	}
	for (i = 0; i < NUM; i++)
	{
		free(tokens[i]);
	}
	fclose(file);
	return 0;
}


