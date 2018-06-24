/*
* Author: Dr. Neerja Mhaskar
* Course: Operating Systems CS 3SH3, Winter 2018
* Example of using semaphores in C on Linux.
* Compile in Linux: gcc -pthread -o lab5b6a lab5b6a.c
* This program creates a total of 10 threads. 7 threads are
* created to run the deposit function and the other 3
* are created to run in the withdraw function.
* Both deposit and withdraw access and modify the shared variable
* amount. Mutex locks are used to lock CS.
* Semaphores are used to ensure the following conditions are met.
* 1) You cannot withdraw if the amount <=0
* 2) You cannot deposit if the amount >= 400
*/

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define NUMDepositThreads 7
#define NUMWithdrawThreads 3
#define true 1
#define LIMIT 400
/*Declare mutex*/
pthread_mutex_t mutex;
/*Declare semaphores*/

sem_t empty;
sem_t full;

void *deposit(void *param);
void *withdraw(void *param);
/*Shared Variable*/
int amount=0;

int main(int argc, char *argv[])
{
	int i, j, max;
	
	pthread_t deposittid[NUMDepositThreads];
	pthread_t withdrawtid[NUMWithdrawThreads];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	
	if (argc !=3){
		printf("usage: a.out <deposit integer value> <withdraw integer value> \n");
		return -1;	
	}
	
	if (atoi(argv[1]) < 0){
		printf("%d must be >=0 \n", atoi(argv[1]));	
	}
	if (atoi(argv[2]) < 0){
		printf("%d must be >=0 \n", atoi(argv[2]));	
	}
	/*Initialize mutex*/
	if (pthread_mutex_init(&mutex, NULL) !=0){
		printf("Error in initializing mutex \n");	
	}
	/*Initialize semaphore full to value 0*/
	if (sem_init(&full, 0, 0) !=0){
		printf("Error in initializing semaphore full \n");	
	}
	/*Since 400 is the max . value of amount at any given
	and because we always deposit/withdraw only 100 at any given time
	if the amount = 0, at most max=400/100=4 times can the deposit 
	thread execute before any withdrawals take place. */
	max = LIMIT/100;
	/*Initialize semaphore empty to value 0*/
	if (sem_init(&empty, 0, max) !=0){
		printf("Error in initializing semaphore full \n");	
	}
	/* Create the deposit threads */
	for(i = 0; i < NUMDepositThreads; i++)
	{
		if(pthread_create(&deposittid[i], &attr, deposit, argv[1])!=0)
			printf("Error in Deposit Thread %d \n", i);
	}
	for(i = 0; i < NUMWithdrawThreads; i++)
	{
		if(pthread_create(&withdrawtid[i], &attr, withdraw, argv[2])!=0)
			printf("Error in Withdraw Thread %d \n", i);
	}
	for(i = 0; i < NUMDepositThreads; i++)
	{
		pthread_join(deposittid[i], NULL);		
	}
	for(i = 0; i < NUMWithdrawThreads; i++)
	{
		pthread_join(withdrawtid[i], NULL);
		
	}

	printf("Final amount = %d \n", amount);	
	return 0;
}

void *deposit(void *param) {
	printf("Executing deposit function\n");
	while (true) { /* deposit 100 */
		int v = atoi(param);
		sem_wait(&empty);
		/*Acquire mutex lock*/
		pthread_mutex_lock(&mutex);				
		amount = amount + v;
		printf("Amount after deposit = %d \n", amount);	
		/*Release mutex locks*/
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
		pthread_exit(0);
	}
	
}

void *withdraw(void *param) {
	printf("Executing Withdraw function\n");
	while (true) { /* withdraw 50 */
		int v = atoi(param);
		sem_wait(&full);  
		/*Acquire mutex lock*/
		pthread_mutex_lock(&mutex);
		amount = amount - v;	
		printf("Amount after Withdrawal = %d \n", amount);	
		/*Release mutex locks*/
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
		pthread_exit(0);
	}
	
}

