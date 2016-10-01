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
/*
 * FIXIT:
 * 128 - магическое число
 */
  char buffer[128];
  
  /*
   * FIXIT:
   * если хотите приложить к программе пример, то закоммитьте дополнительный файл test.txt
   * + можно приложить файл readme.txt с комментарием использования...
   * можно было бы написать код, будто считывается с консоли, а запускать так 
   * ./a.out < test.txt
   * 
   * В код программы включать генерацию примера не нужно.
   */
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
      /*
       * FIXIT:
       * представьте, что требуется запустить задачу с несуществующим названием ...
       * вы создаете новый процесс под это, а execvp возвращает какую-то ошибку ...
       * дальше уже два процесса будут запускать одни и те же задачи ...
       */
		}
	}

	/*
   * Можно было бы добавить код, чтобы запускающий процесс дождался завершения всех дочерних
   */
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
  
  /*
   * Можно не править, но просто представьте, что вам надо использовать чужую ф-ю, которая в случае ошибки печатает в консоль какой-то текст.
   * Вы как ответственный программист хотите обрабатывать все пробленые случаи. У вас просто никак не получится это сделать.
   * 
   * Если хотите обрабатывать ошибки, то ф-я должна возвращать код ошибки, например.
   */
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

/*
 * В целом код хороший. Нужно только поправить небольшие замечания.
 */
