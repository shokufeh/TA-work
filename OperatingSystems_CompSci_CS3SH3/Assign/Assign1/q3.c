/*
* Author: Dr. Neerja Mhaskar
* Course: Operating Systems CS 3SH3, Winter 2018
* Solution to Practice lab 3a/3b
* Compile in Linux: gcc -pthread -o pl3a3b pl3a3b
* This program creates a total of 2 threads each returning the 
* sum of the numbers in the sublist assigned to them.
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

void *printodd();
void *printeven();

/*Parameter data structure created to store a range in the list.*/

int main(int argc, char *argv[])
{
	int i;
	pthread_t workers[NUMThreads];
	pthread_attr_t attr;
	 
	/* establish the first summing thread */
	pthread_attr_init(&attr);
	printf("Numbers from 1- %d\n", atoi(argv[1]));
	/*Initialize mutex*/
	if (pthread_mutex_init(&mutex, NULL) !=0){
		printf("Error in initializing mutex \n");	
	}
	/*Initialize semaphore full to value 0*/
	if (sem_init(&seven, 0, 1) !=0){
		printf("Error in initializing semaphore seven \n");	
	}
	/*Since bot the threads need to alternate executing in there respective 
	functions, the max.no.of times the threads can execute before the other 
	executes is 1. */
	
	/*Initialize semaphore empty to value 0*/
	if (sem_init(&sodd, 0, 0) !=0){
		printf("Error in initializing semaphore full \n");	
	}
	pthread_create(&workers[0], &attr, printodd, argv[1]);	
	/* establish the second summing thread */
	
	pthread_create(&workers[1], &attr, printeven,argv[1]);
	/* now wait for the 2 summing threads to finish */
	for (i = 0; i < NUMThreads - 1; i++)
		pthread_join(workers[i], NULL);
	/* output the sum */
    return 0;
}

/**
 *print odd thread.
 *
 */

void *printodd(void *params)
{
	int i;
	int n= atoi(params);
	if(n%2==0){n=n/2 -1;}
	else{n= (n-1)/2;}
	while (true) { /* deposit 100 */
		int i=0;		
		while(i<=n){
			sem_wait(&seven);
			/*Acquire mutex lock*/
			pthread_mutex_lock(&mutex);			
			printf("%d \n", 2*i+1);	
			i=i+1;
			/*Release mutex locks*/
			pthread_mutex_unlock(&mutex);
			sem_post(&sodd);
		}
		pthread_exit(0);
				
	}
}

void *printeven(void *params)
{
	int i;
	int n= atoi(params);
	if(n%2==0){n=n/2;}
	else{n= (n-1)/2;}
	while (true) { /* deposit 100 */
		int i=1;
		while(i<=n){
			sem_wait(&sodd);
			/*Acquire mutex lock*/
			pthread_mutex_lock(&mutex);				
			printf("%d \n", 2*i);	
			i=i+1;
			/*Release mutex locks*/
			pthread_mutex_unlock(&mutex);
			sem_post(&seven);
		}
		pthread_exit(0);
				
	}
}
	


