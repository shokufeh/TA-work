/*
* Author: Dr. Neerja Mhaskar
* Course: Operating Systems CS 3SH3, Winter 2018
* Solution to Practice lab 3a/3b
* Compile in Linux: gcc -pthread -o q3 q3.c
* Solution to Assignment 1, Q3 using just semaphores.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NUMThreads 2
#define true 1
/*Declare semaphores*/
sem_t seven;
sem_t sodd;

void *printodd();
void *printeven();

int main(int argc, char *argv[])
{
	int i;
	pthread_t workers[NUMThreads];
	pthread_attr_t attr;
	 
	/* establish the thread attributes */
	pthread_attr_init(&attr);
	printf("Numbers from 1- %d\n", atoi(argv[1]));
	/*Initialize semaphore seven to value 1*/
	if (sem_init(&seven, 0, 1) !=0){
		printf("Error in initializing semaphore seven \n");	
	}
	/*Since bot the threads need to alternate executing in there respective 
	functions, the max.no.of times the threads can execute before the other 
	executes is 1. */
	
	/*Initialize semaphore sodd to value 0*/
	if (sem_init(&sodd, 0, 0) !=0){
		printf("Error in initializing semaphore full \n");	
	}
	/* establish the first thread */
	pthread_create(&workers[0], &attr, printodd, argv[1]);	
	
	/* establish the second thread */
	pthread_create(&workers[1], &attr, printeven,argv[1]);
	/* now wait for the 2 threads to finish */
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
	int i;
	int n= atoi(params);
	if(n%2==0){n=n/2 -1;}
	else{n= (n-1)/2;}
	while (true) { 
		int i=0;		
		while(i<=n){
			sem_wait(&seven);			
			printf("%d \n", 2*i+1);	
			i=i+1;
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
	while (true) { 
		int i=1;
		while(i<=n){
			sem_wait(&sodd);				
			printf("%d \n", 2*i);	
			i=i+1;
			sem_post(&seven);
		}
		pthread_exit(0);
				
	}
}
	


