#include <stdio.h>
#include <string.h>

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
}


int main()
{
	char* string = (char*) malloc (100 * sizeof(char));
	char* delimiters = (char*) malloc (100 * sizeof(char));
	gets(string);
	gets(delimiters);
	char** tokens = (char**) malloc (sizeof(char*) * 100);
	
	int i = 0;
	for(i = 0; i < 100; i++)
	{
		tokens[i] = (char*)malloc(100 * sizeof(char));
	}
	
	int tokensCount = 0;
	Split(str, delimiters, &tokens, &tokensCount);
	
	for(i = 0; i < tokensCount; i++)
	{
		printf("%s\n", tokens[i]);
	}
	
	for(i = 0; i < 100; i++)
	{
		if(tokens[i])
		free(tokens[i]);
	}

	free(tokens);
	free(string);
	free(delimiters);
	return 0;
}
