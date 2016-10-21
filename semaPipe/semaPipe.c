#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>

void semDo(int a, int semId);
struct sembuf mybuf;
int semid;

int main(){
	int a, b, answer;

	int fd[2], result;
	size_t size;
	
	key_t key;

	const char pathname1[] = "key.txt";

	if(pipe(fd) < 0){
		printf("Can\'t create pipe\n");
		exit(-1); 
	} 

	if ( (key = ftok(pathname1 , 0)) < 0 ) {
        printf("Can`t generate key1\n");
        exit(-1);
    }
    
    if ((semid = semget(key , 2 , 0666 | IPC_CREAT)) < 0) {
        printf("Can`t get semid\n");
        exit(-1);
    }
   	
   	semDo(1,1);

	result = fork(); 

	if (result > 0) {
		while(1){
			
			a = rand() % 10;
			b = rand() % 10;
			printf("a - %d b - %d\n", a, b);
			write(fd[1], &a, sizeof(a));
			write(fd[1], &b, sizeof(b));
			semDo(-1, 0);
			read (fd[0], &result, sizeof(result));
			printf("result - %d\n", result);
			semDo(1, 1);
		}


	} else {
		while(1){
			semDo(-1, 1);
			int x,y;
			read(fd[0], &x,sizeof(x));
			read(fd[0], &y,sizeof(y));
			printf("x - %d y - %d\n", x, y);
			answer = x + y;
			write(fd[1], &answer,sizeof(answer));
			semDo(1, 0);
		}
	}
	return 0;
}

void semDo(int a, int semId) {
    
    mybuf.sem_num = semId;
    mybuf.sem_op = a;
    mybuf.sem_flg = 0;
   
    if (semop(semid , &mybuf , 1) < 0) {
        printf("Can`t wait for condition\n");
    	exit(-1);
    }
}