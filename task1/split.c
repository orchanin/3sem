#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS_COUNT 50
#define MAX_STR_LEN 1000

void Split(char* string, char* delimiters,
		   char*** tokens, int* tokensCount);

int main(int argc, char const *argv[])
{
	char* str = (char*)malloc(MAX_STR_LEN * sizeof(char));
	char* delimiters = (char*)malloc(MAX_TOKENS_COUNT * sizeof(char));
	gets(str);
	gets(delimiters);
	char** tokens = (char**)malloc(sizeof(char*) * MAX_TOKENS_COUNT);
	int i = 0;
	for(i = 0; i < MAX_TOKENS_COUNT; i++)
	{
		tokens[i] = (char*)malloc(MAX_TOKENS_COUNT * sizeof(char));
	}
	int tokensCount = 0;
	
	Split(str, delimiters, &tokens, &tokensCount);
	
	for(i = 0; i < tokensCount; i++)
	{
		printf("%s\n", tokens[i]);
	}
	for(i = 0; i < MAX_TOKENS_COUNT; i++)
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
		printf("error\n");
		return;
	}
	while (temp != NULL){
		strcpy((matrix)[*tokensCount], temp);
		(*tokensCount)++;
		temp = strtok(NULL, delimiters);
	}
}
