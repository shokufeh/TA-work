/*
* Author: Dr. Neerja Mhaskar
* Course: Operating Systems CS 3SH3, Winter 2018
* Example of using mutex locks provided by the Pthreads API.
* Compile in Linux: gcc -pthread -o lab5a6a lab5a6a.c
* This program creates a total of 6 threads. 3 threads are
* created to run the deposit function and the other 3
* are created to run in the withdraw function.
* Both deposit and withdraw access and modify the shared variable
* amount.
*/

#include <pthread.h>
#include <stdio.h>

#define NUMThreads 3
#define true 1
/*Declare mutex*/
pthread_mutex_t mutex;

void *deposit(void *param);
void *withdraw(void *param);
/*Shared Variable*/
int amount=0;
//int atoi(const char *str);

int main(int argc, char *argv[])
{
	int i, j;
	
	pthread_t deposittid[NUMThreads];
	pthread_t withdrawtid[NUMThreads];
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
	/* Create the deposit threads */
	for(i = 0; i < NUMThreads; i++)
	{
		if(pthread_create(&deposittid[i], &attr, deposit, argv[1])!=0)
			printf("Error in Deposit Thread %d \n", i);
	}
	for(i = 0; i < NUMThreads; i++)
	{
		if(pthread_create(&withdrawtid[i], &attr, withdraw, argv[2])!=0)
			printf("Error in Withdraw Thread %d \n", i);
	}
	for(i = 0; i < NUMThreads; i++)
	{
		pthread_join(deposittid[i], NULL);
		pthread_join(withdrawtid[i], NULL);
	}

	printf("Final amount = %d \n", amount);	
	return 0;
}

void *deposit(void *param) {
	//printf("I am in deposit \n");
	while (true) { /* deposit 100 */
		int v = atoi(param);
		/*Acquire mutex lock*/
		pthread_mutex_lock(&mutex);		
		amount = amount + v;
		printf("Deposit amount = %d \n", amount);	
		/*Release mutex locks*/
		pthread_mutex_unlock(&mutex);
		pthread_exit(0);
	}
	
}

void *withdraw(void *param) {
	//printf("I am in Withdraw \n");
	while (true) { /* withdraw 50 */
		int v = atoi(param);
		/*Acquire mutex lock*/
		pthread_mutex_lock(&mutex);
		amount = amount - v;	
		printf("Withdrawal amount = %d \n", amount);	
		/*Release mutex locks*/
		pthread_mutex_unlock(&mutex);
		pthread_exit(0);
	}
	
}

