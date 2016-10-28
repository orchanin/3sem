#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DATA_TYPE 255

int main() {

	int msqid; 
	char pathname[] = "s06e01a.c";
	key_t key; 
	struct mymsgbuf
  	{	
    	long mtype;
    	struct {
    		int a;
    		int b;
    		pid_t type;
    	} info;
  	} mybuf;

 	struct resultbuf
  	{	
    	long mtype;
    	int c;
  	} result;

  	mybuf.info.a = rand() % 10;
  	mybuf.info.b = rand() % 10;
  	mybuf.mtype = DATA_TYPE;

  	printf("a = %d b = %d\n", mybuf.info.a, mybuf.info.b);
  	pid_t type = getpid();
  	mybuf.info.type = type;

  	if ((key = ftok(pathname, 0)) < 0)
  	{
    	printf("Can\'t generate key\n");
    	exit(-1);
  	}
  	if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
  	{
    	printf("Can\'t get msqid\n");
    	exit(-1);
  	}

  	if (msgsnd(msqid, (struct msgbuf *) &mybuf, sizeof(mybuf.info), 0) < 0)
    {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
        exit(-1);
    }
    printf("Waiting for server\n");
    msgrcv(msqid, (struct msgbuf *)&result, sizeof(result.c), type, 0);
    printf("a + b = %d\n", result.c);
}