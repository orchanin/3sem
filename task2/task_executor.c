#include "stdio.h"
#include "unistd.h"
#include "sys/wait.h"
#include "stdlib.h"
#include "string.h"
#define MAX_TOKENS_COUNT 50

void Split(char* string, char* delimiters,
		   char*** tokens, int* tokensCount);

int main(int argc, char const *argv[])
{
	FILE *file;
	char buffer[128];
	file = fopen("test.txt", "w");

	fprintf(file, "3\n");
	fprintf(file, "3 ls -l\n");
	fprintf(file, "4 pwd\n");
	fprintf(file, "5 echo Hello, World !\n");
	fclose(file);
	file = fopen("test.txt", "r");

	char** tokens = (char**)malloc(sizeof(char*) * MAX_TOKENS_COUNT);
	int i = 0;
	for(i = 0; i < MAX_TOKENS_COUNT; i++)
	{
		tokens[i] = (char*)malloc(MAX_TOKENS_COUNT * sizeof(char));
	}
	int tokensCount = 0;
	char delimiters[2] = {' ','\n'};
	fgets(buffer, 128, file);
	int count = atoi(buffer);

	for (int i = 0; i < count; i++)
	{
		fgets(buffer, 128, file);
		pid_t pid = fork();
		if (pid == 0)
		{
			Split(buffer, delimiters, &tokens, &tokensCount);
			sleep(atoi(tokens[0]));
			execvp(tokens[1], tokens + 1);
		}
	}

	for(i = 0; i < MAX_TOKENS_COUNT; i++)
	{
		free(tokens[i]);
	}
	fclose(file);
	return 0; 
}
void Split(char* string, char* delimiters,
		   char*** tokens, int* tokensCount)
{	
	char** matrix = *tokens;
	char* temp = NULL;
	temp = strtok(string, delimiters);
	if (temp == NULL)
	{
		printf("error\n");
		return;
	}
	while (temp != NULL)
	{
		strcpy((matrix)[*tokensCount], temp);
		(*tokensCount)++;
		temp = strtok(NULL, delimiters);
	}
	(matrix)[*tokensCount] = NULL;
}
