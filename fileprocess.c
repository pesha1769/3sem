#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#define MAX_CHARS_AMOUNT 100
#define SIZE 128
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
	char buffer[SIZE];
  	FILE *file;
	file = fopen("commands.txt", "r");
		
	char** tokens = (char**)malloc(sizeof(char*) * MAX_CHARS_AMOUNT);

	for (i = 0; i < MAX_CHARS_AMOUNT; i++)
	{
		tokens[i] = (char*)malloc(MAX_CHARS_AMOUNT * sizeof(char));
	}
	int tokensCount = 0;
	char delimiters[2] = {' ', '\n'};
	fgets(buffer, SIZE, file);
	int count = atoi(buffer);
	for (i = 0; i < count; i++)
	{

		fgets(buffer, SIZE, file);
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
	for (i = 0; i < MAX_CHARS_AMOUNT; i++)
	{
		free(tokens[i]);
	}
	fclose(file);
	return 0;
}


