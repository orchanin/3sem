#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void send_symbol(int * symbol);
void print_symbol(int *begin);
void semDo(int a);

char* string = "Mother washes the window!";

int currentBit = 0;

int semid;
key_t key;
struct sembuf mybuf;
pid_t pid;
pid_t ppid;


int point = 0;
int bitSymbol[8];

void recievе_0() {
	
	bitSymbol[point] = 0;
	point++;
	
	if (point > 7) {
		point = 0;
		print_symbol(bitSymbol);
	}
	semDo(1);
}

void recievе_1() {
	
	bitSymbol[point] = 1;
	point++;	

	if (point > 7) {
		point = 0;
		print_symbol(bitSymbol);
	}
	semDo(1);
}          

int main(int argc, char const *argv[])
{

	int length = strlen(string);
	
	const char pathname[] = "semaf1.txt";

	if ( (key = ftok(pathname , 0)) < 0 ) {
        printf("Can`t generate key1\n");
        exit(-1);
    }
    
    if ((semid = semget(key , 1 , 0666 | IPC_CREAT)) < 0) {
        printf("Can`t get semid\n");
        exit(-1);
    }

    semctl(semid, 0, SETVAL, 0);


	pid = fork();

	if (pid == 0) {
		ppid = getppid();
		
		(void)signal(SIGUSR1, recievе_0);
		(void)signal(SIGUSR2, recievе_1);

		while(1);
	} 
	if (pid > 0) {

		sleep(1);

		for (int i = 0; i < length; i++)
		{
			send_symbol((int *)&string[i]);
		}
	}
	kill(pid, 9);
	printf("\n");
	return 0;
}

void send_symbol(int *symbol){
	for (int i = 128; i > 0; i = i / 2){

		if (*symbol & i){
			kill (pid, SIGUSR2);
		} else {
			kill (pid, SIGUSR1);
		}
		semDo(-1);
	}
}

void print_symbol(int *begin){	
	
	int asci = 0;
	for (int i = 128; i > 0	; i = i / 2)
	{
		asci += *(begin) * i;
		begin++;
	}           
	printf("%c", (char)asci);
	fflush(stdout);
}

void semDo(int a) {
	
	mybuf.sem_num = 0;
	mybuf.sem_op = a;
	mybuf.sem_flg = 0;
   
	if (semop(semid , &mybuf , 1) < 0) {
		printf("Can`t wait for condition\n");
		exit(-1);
	}
}
