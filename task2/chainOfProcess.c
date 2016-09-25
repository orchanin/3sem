#include "stdio.h"
#include "unistd.h"
#include "sys/wait.h"
#include "stdlib.h"
int main(int argc, char const *argv[])
{
	int N;
	
	scanf("%d", &N);
	int count = 0;
	int status = 0;
	
	pid_t currentParId = getpid();
	
	for (int i = 0; i < N - 1; ++i)
	{
		
		if (getpid() == currentParId){
			
			printf("Process № %d\nppid - %d\n",getpid(),getppid());
			sleep(1);
			pid_t pid = fork();
			
			if(getpid() == currentParId)
			{

				printf("Waiting for finishing Process № %d\n\n", pid);
				wait(&status);
				printf("Process №%d is finishing\n", getpid());
				exit(0);

			}
			
			currentParId = getpid();
		}
	}
	printf("Process № %d\nppid - %d\n\n",getpid(),getppid());
	printf("Process №%d is finishing\n", getpid());
	return 0;
}
