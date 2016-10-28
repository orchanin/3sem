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


    
    fflush(stdout);
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

	FILE *washer = fopen("washer.txt", "r");
	FILE *dishes = fopen("dishes.txt", "r");

	int table_fd = open("table.txt",O_RDWR | O_CREAT);  // shared file for both processes

	if((key = ftok(pathname,0)) < 0){
        printf("Can't generate key\n");
        exit(-1);
    }
    if ((semid = semget(key, 1, 0666|IPC_CREAT)) < 0) {
        printf("Can't create semaphore set\n");
        exit(-1);
    }

    //reciving information from files
    int max = 0;
    int temp1, temp2;

    while (2 == fscanf(washer, "%d:%d\n", &temp1, &temp2)) {
        if (temp1 > max) max = temp1;
    }
    rewind(washer);
    int *speed = (int *) malloc (sizeof(int) * max);
    while (fscanf(washer, "%d:%d\n", &temp1, &temp2) == 2) {
        speed[temp1] = temp2;      //array of power of washer
    }
    fclose(washer);

    int end = 0;
    int table = 0;

    while (end == 0) {
        table = semctl(semid, 0, GETVAL);
        
        //washer wait to emty space on the table if the table is full
        while (table == limit) {
            table = semctl(semid, 0, GETVAL);
        }

        while (table < limit) {
            int amount;
            int dish;
            char buf[5];
            
            fscanf(dishes, "%d:%d", &dish, &amount);
            

            for (int i = 0; i < amount; i++)
            {
                table = semctl(semid, 0, GETVAL);
                //washer wait to emty space on the table if the table is full
                while (table == limit) {
                    table = semctl(semid, 0, GETVAL);
                }

                sleep(speed[dish]);
                printf("Washer has washed dish - №%d and put it on table\n", dish);

                
                sprintf(buf, "%d\n", dish);
                write(table_fd, buf, 3);

                semDo(1);

            }
            if (dish == 0) {
                end = 1;
                sprintf(buf, "%d\n", dish);
                write(table_fd, buf, 3);
                break;
            }
            table = semctl(semid, 0, GETVAL);
        }
    }   
    write(table_fd, 0, 3);
    close(table_fd);
    fclose(dishes);
    printf("Washer has finished his work\n");
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