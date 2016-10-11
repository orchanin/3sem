#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define DATA_SIZE 100000000	

/*************************************
/ Set here number of cores of your CPU
*/
#define NUMBER_OF_CORES 2		
//************************************

/*
* Вроде работает правильно, но ускорение в нитях выдает время от времени
*/

struct Segment {
	int begin;
	int end;
	double sum;
};

double data[DATA_SIZE];
void* sum(void* arg);
void* disp(void* arg);

int main(int argc, char const *argv[]){
	
	srand(time(NULL));

	for (int i = 0; i < DATA_SIZE; i++)
	{
		data[i] = (double) (rand() % 2);
	}

//****************************************************************************
//  Avarage without threads
//****************************************************************************
    double avarage = 0;
    
    clock_t begin = clock();

	for (int i = 0; i < DATA_SIZE; i++)
	{
		avarage  = avarage + data[i];
	}
    avarage = avarage / DATA_SIZE;

    clock_t end = clock();
    double timeSpent1 = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("Avarage - %lg\nAvarage time without threads - %lg\n\n", avarage, timeSpent1);
//****************************************************************************
    avarage = 0;

    pthread_t threads[NUMBER_OF_CORES];

    
    struct Segment* segments = (struct Segment*)calloc( NUMBER_OF_CORES, sizeof(struct  Segment));
    
    for (int i = 0; i < NUMBER_OF_CORES; i++)
    {
        segments[i].begin = i * DATA_SIZE / NUMBER_OF_CORES;
        segments[i].end = (i + 1) * DATA_SIZE / NUMBER_OF_CORES;
        segments[i].sum = 0;
    }
//****************************************************************************
//  Avarage with threads
//****************************************************************************
    begin = clock();
	
    for (int i = 0; i < NUMBER_OF_CORES; i++)
	{
	   pthread_create(&(threads[i]) , 
                    (pthread_attr_t*)NULL, 
                    sum,
                    (void*)&(segments[i]));
	}
    for (int i = 0; i < NUMBER_OF_CORES; i++)
    {
        pthread_join(threads[i], (void **) NULL);  
        avarage += segments[i].sum;      
    }
    avarage = avarage / DATA_SIZE;

    end = clock();
    double timeSpent2 = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Avarage - %lg\nAvarage time with threads - %lg\n\n", avarage, timeSpent2);
    double acceleration = timeSpent1 / timeSpent2;
    if (acceleration > 1)    
    {
        printf("Threads work good =) Avarage was calculated faster in %lg times\n\n", acceleration);
    }
    else 
    {
        printf("Threads work bad =(\n\n");
    }
//****************************************************************************
// Dispersion without Threads
//****************************************************************************
    double dispersion = 0;

    begin = clock();
    for (int i = 0; i < DATA_SIZE; ++i)
    {
        dispersion += data[i] * data[i];
    }
    dispersion = dispersion / DATA_SIZE - avarage * avarage;
    
    end = clock();
    double timeSpent3 = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Dispersion - %lg\nDispersion time without Threads - %lg\n\n",dispersion, timeSpent3 );
//****************************************************************************
// Dispersion with Threads
//****************************************************************************
    dispersion = 0;

    begin = clock();
    for (int i = 0; i < NUMBER_OF_CORES; i++)
    {
       pthread_create(&(threads[i]) , 
                    (pthread_attr_t*)NULL, 
                    disp,
                    (void*)&(segments[i]));
    }
    for (int i = 0; i < NUMBER_OF_CORES; i++)
    {
        pthread_join(threads[i], (void **) NULL);  
        dispersion += segments[i].sum;      
    }
    dispersion = dispersion / DATA_SIZE - avarage * avarage;
    
    end = clock();
    double timeSpent4 = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Dispersion - %lg\nDispersion time with Threads - %lg\n\n",dispersion, timeSpent4 );
	
    acceleration = timeSpent3 / timeSpent4;
    if (acceleration > 1)    
    {
        printf("Threads work good =) Dispersion was calculated faster in %lg times\n\n", acceleration);
    }
    else 
    {
        printf("Threads work bad =(\n\n");
    }

	return 0;
}

void* sum(void* arg) {
    struct Segment* thread_segment = (struct Segment*)(arg); 
    int begin = thread_segment -> begin;
    int end = thread_segment -> end;
    double result = 0;

    for (int i = begin ; i < end; i++)
    {
        result += data[i];
    }
   
    thread_segment -> sum = result;
    
    return NULL;
}

void* disp(void* arg) {
    struct Segment* thread_segment = (struct Segment*)(arg); 
    int begin = thread_segment -> begin;
    int end = thread_segment -> end;
    double result = 0;

    for (int i = begin ; i < end; i++)
    {
        result += data[i]*data[i];
    }
   
    thread_segment -> sum = result;
    
    return NULL;
}

