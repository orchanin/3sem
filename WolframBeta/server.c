#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>

#define DATA_TYPE 255
#define MAX_CALCULATE_THREADS 2

int msqid; 
int semid;

void semDo(int a);
struct sembuf semobuf;


struct mymsgbuf    {   
    long mtype;
    struct {            
        int a;
        int b;
        pid_t type;
    } info;
};

struct resultbuf    {   
    long mtype;
    int c;
} result;

void* my_thread1(void* dummy) {
    semDo(-1);
    struct mymsgbuf buf = *(struct mymsgbuf *) (dummy);
    int a = buf.info.a;
    int b = buf.info.b;
    printf("In thread a = %d b = %d\n", a, b);
    printf("Calculating...\n");
    sleep(5);

    struct resultbuf res;
    res.mtype = buf.info.type;
    res.c = a + b;

    if (msgsnd(msqid, (struct resultbuf *) &res, sizeof(result.c), 0) < 0)
    {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
        exit(-1);
    }
    printf("Easy\n");
    semDo(1);
    return NULL;
}
int main() {
    pthread_t thread_id;
    char pathname[] = "s06e01a.c";
    key_t key; 


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
    if ((semid = semget(key, 1, 0666|IPC_CREAT)) < 0) {
        printf("Can't create semaphore set\n");
        exit(-1);
    }
    semDo(MAX_CALCULATE_THREADS);
    
    struct mymsgbuf mybuf;
    while(1) {
        
        msgrcv(msqid, (struct msgbuf *)&mybuf, sizeof(mybuf.info), DATA_TYPE, 0);
        
        int result = pthread_create(&thread_id, 
                            (pthread_attr_t *)NULL , 
                            my_thread1 ,
                            &mybuf);
    }
}
void semDo(int a) {
    
    semobuf.sem_num = 0;
    semobuf.sem_op = a;
    semobuf.sem_flg = 0;
   
    if (semop(semid , &semobuf , 1) < 0) {
        printf("Can`t wait for condition\n");
        exit(-1);
    }
}