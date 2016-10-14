/*
Напишите две программы, осуществляющие
взаимодействие через разделяемую память.
Первая программа должна создавать сегмент
разделяемой памяти и копировать туда свой
собственный исходный текст, вторая
программа должна брать оттуда этот текст,
печатать его на экране и удалять сегмент
разделяемой памяти из системы.
*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define SIZE 100000 

int main(int argc, char const *argv[])
{
	 char* array;
	 int shmid;
	 char buffer[50];
	 const char pathname[] = "shmemoryTask.c";
	 const char pathnameForKey[] = "shmemoryTask.c";
	 int bytesRead;
	 key_t key;

	 if((key = ftok(pathnameForKey , 0)) < 0){ /* Генерируем ключ */
       	printf("Can\'t generate key\n");
        exit(-1);
  	}

  	if((shmid = shmget(key, SIZE * sizeof(char), 0666 | IPC_CREAT | IPC_EXCL)) < 0){    
        /*В случае ошибки пытаемся определить возникла ли она из-за того, что 
        сегмент разделяемой памяти уже существует или по другой причине */
        if(errno != EEXIST) {         
            /* По другой причине...*/
            printf("Can't create shared memory\n");
            exit(-1);
        } else {
        /* Если из-за того, что разделяемая память уже существует, то пытаемся получить 
            её IPC дескриптор и, в случае удачи, сбрасываем флаг необходимости
            инициализации элементов массива
        */
        	if((shmid = shmget(key, SIZE * sizeof(char), 0)) < 0){
            	printf("Can't find shared memory\n");
            	exit(-1);
        	}
        }	
    }
    if((array = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)){
    	printf("Can't attach shared memory\n");
      	exit(-1);
   	}

   	int fd = open(pathname, O_RDONLY);
   	int len = 0;
   	
   	while ((bytesRead = read(fd,buffer, 50)) > 0){
   	    for (int i = 0; i < bytesRead; i++)
   	    {  
   		   	array[i + len] = buffer[i];
   	    }
   		len += bytesRead;
   	}
    close(fd);

   	if(shmdt(array) < 0){
        printf("Can't detach shared memory\n");
        exit(-1);
    }


	return 0;
}