#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void semDo(int a);
struct sembuf mybuf;
int semid;

int main(int argc, char *argv[], char *envp[]) {

	int i = 0;
	int limit = 0;
	char pathname[] = "readme.txt";
	key_t key;
	

	while(envp[i] != NULL){
		if (strstr(envp[i], "TABLE_LIMIT") != NULL) {
			sscanf(envp[i], "TABLE_LIMIT=%d", &limit);	
            break;
		}
        i++;
	}

	FILE *cleaner = fopen("cleaner.txt", "r");

	int table_fd = open("table.txt",O_RDWR | O_CREAT);  // shared file for both processes

	if((key = ftok(pathname,0)) < 0){
        printf("Can't generate key\n");
        exit(-1);
    }
    if ((semid = semget(key, 1, 0666|IPC_CREAT)) < 0) {
        printf("Can't create semaphore set\n");
        exit(-1);
    }

    //reciving information about cleaner speed from file
    int max = 0;
    int temp1, temp2;

    while (2 == fscanf(cleaner, "%d:%d\n", &temp1, &temp2)) {
        if (temp1 > max) max = temp1;
    }
    rewind(cleaner);
    int *speed = (int *) malloc (sizeof(int) * max);
    while (fscanf(cleaner, "%d:%d\n", &temp1, &temp2) == 2) {
        speed[temp1] = temp2;      //array of power of cleaner
    }
    fclose(cleaner);

    int itemNumber;
    int table = 0;
    char buf[10];

    while (1) {
        table = semctl(semid, 0, GETVAL);
        
        char buf[3];
        read(table_fd, buf, 3);
        
        sscanf(buf, "%d", &itemNumber);
        if (itemNumber == 0) {
            printf("Cleaner has finished.\n");
            break;
        }
        sleep(speed[itemNumber]);
        printf("Cleaner has cleaned dish №%d and put it away from table\n", itemNumber);
        
        semDo(-1);
        while (table == 0) {
            table = semctl(semid, 0, GETVAL);
        }
    }   
    close(table_fd);
    return 0;
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