/*
* Author: Dr. Neerja Mhaskar
* Course: Operating Systems CS 3SH3, Winter 2018
* Solution to Practice lab 3a/3b
* Compile in Linux: gcc -pthread -o pl3a3b pl3a3b
* Solution using mutex and semaphores to Assignment 1, Q3.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NUMThreads 2
#define true 1

/*Declare mutex*/
pthread_mutex_t mutex;
/*Declare semaphores*/
sem_t seven;
sem_t sodd;

/*Parameter data structure to pass values to the thread*/
typedef struct threadparam {
	int limit;
	int current;
}parameters;

void *printodd();
void *printeven();

int main(void)
{
	int i;
	pthread_t workers[NUMThreads];
	pthread_attr_t attr;

	 
	/* establish the first thread attributes */
	pthread_attr_init(&attr);
	printf("Numbers from 1- %d\n", 100);

	/*Initialize mutex*/
	if (pthread_mutex_init(&mutex, NULL) !=0){
		printf("Error in initializing mutex \n");	
	}
	/*Initialize semaphore seven to value 0*/
	if (sem_init(&seven, 0, 1) !=0){
		printf("Error in initializing semaphore seven \n");	
	}
	/*Since both the threads need to alternate executing in there respective 
	functions, the max.no.of times the threads can execute before the other 
	executes is 1. */	
	
	/*Initialize semaphore sodd to value 0*/
	if (sem_init(&sodd, 0, 0) !=0){
		printf("Error in initializing semaphore full \n");	
	}
	parameters *data = (parameters *) malloc (sizeof(parameters));
	data->limit = 100;
	data->current = 0;
	
	/* establish the first thread */
	pthread_create(&workers[0], &attr, printodd, data);	
	
	/* establish the second thread */
	pthread_create(&workers[1], &attr, printeven,data);

	/* wait for the 2 threads to finish */
	for (i = 0; i < NUMThreads - 1; i++)
		pthread_join(workers[i], NULL);
    return 0;
}

/**
 *print odd thread.
 *
 */

void *printodd(void *params)
{	
	while(true){
		sem_wait(&seven);
		/*Acquire mutex lock*/
		pthread_mutex_lock(&mutex);
		parameters* p = (parameters *)params;
		//int limit = p->limit;
		//int current = p->current;
		if(p->current < p->limit){
			p->current = p->current + 1;			
			printf("%d \n", p->current);		
		}
		else{					
			pthread_exit(0);
				
		}
		/*Release mutex locks*/
		pthread_mutex_unlock(&mutex);
		sem_post(&sodd);
	}
}

void *printeven(void *params)
{	
	while(true){
		sem_wait(&sodd);
		/*Acquire mutex lock*/
		pthread_mutex_lock(&mutex);
		parameters* p = (parameters *)params;
		//int limit = p->limit;
		//int current = p->current;
		if(p->current <= p->limit){
			p->current = p->current + 1;			
			printf("%d \n", p->current);		
		}	
		else{					
			pthread_exit(0);
				
		}
		/*Release mutex locks*/
		pthread_mutex_unlock(&mutex);
		sem_post(&seven);
	}
}

	


