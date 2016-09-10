#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER 50

void Split(char* string, char* delimiters,
		   char*** tokens, int* tokensCount);

int main(int argc, char const *argv[])
{
	char* str = (char*)malloc(10000 * sizeof(char));
	char* delimiters = (char*)malloc(NUMBER * sizeof(char));
	gets(str);
	gets(delimiters);
	char** tokens = (char**)malloc(sizeof(char*) * NUMBER);
	int i = 0;
	for(i = 0; i < NUMBER; i++)
	{
		tokens[i] = (char*)malloc(NUMBER * sizeof(char));
	}
	int tokensCount = 0;
	
	Split(str, delimiters, &tokens, &tokensCount);
	
	for(i = 0; i < tokensCount; i++)
	{
		printf("%s\n", tokens[i]);
	}
	for(i = 0; i < NUMBER; i++)
	{
		if(tokens[i])
			free(tokens[i]);
	}
	free(tokens);
	free(str);
	free(delimiters);

	return 0;
}

void Split(char* string, char* delimiters,
		   char*** tokens, int* tokensCount)
{
	char** matrix = *tokens;					
	char* temp = NULL;
	temp = strtok(string, delimiters);
	if (temp == NULL) {
		printf("eror");
		return;
	}
	while (temp != NULL){
		strcpy((matrix)[*tokensCount], temp);
		(*tokensCount)++;
		temp = strtok(NULL, delimiters);
	}
}