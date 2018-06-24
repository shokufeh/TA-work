/*
* Author: Dr. Neerja Mhaskar
* Course: Operating Systems CS 3SH3, Winter 2018
* Solution to Labtest 2
* Compile in Linux:$ gcc -pthread -o labtest2 labtest2.c
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMThreads 2 //Number of threads running concurrently.

void *sum(void *param);
void *triples(void *param);
void *doubles(void *param);

/*Parameter data structure created to store data that is to be
passed to the thread. Returnval enables a thread to return the computed
value to the main thread without used global variables.*/

typedef struct threadparam {
	int value1;
	int value2;
	int returnval;
}parameters;
int main(int argc, char *argv[])
{
	pthread_t workers[NUMThreads];
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	
	/* establish the first thread */
	parameters *data = (parameters *) malloc (sizeof(parameters));
	data-> value1 = atoi(argv[1]);
	data-> value2 = atoi(argv[2]);
	data-> returnval = 0;	
	pthread_create(&workers[0], &attr, doubles, data);
	
	/* establish the second thread */
	parameters *data1 = (parameters *) malloc (sizeof(parameters));
	data1-> value1 = atoi(argv[3]);
	data1-> value2 = atoi(argv[4]);
	data1-> returnval = 0;	
	pthread_create(&workers[1], &attr, triples, data1);
	
	/* now wait for the two worker threads to finish */
	int i;
	for (i = 0; i < NUMThreads - 1; i++)
		pthread_join(workers[i], NULL);

	/*Data for the third thread*/
	parameters *data2 = (parameters *) malloc (sizeof(parameters));
	data2-> value1 = data-> returnval;
	data2-> value2 = data1-> returnval;
	data2-> returnval = 0;
	pthread_create(&tid, &attr, sum, data2);
	pthread_join(tid, NULL);

	/* output the value returned by thread3 */
	printf("Value returned by Thread3 in main: %d \n",data2->returnval);
	free(data);
	free(data1);
	free(data2);
	
    return 0;
}

/* Thread1 begins execution in this function */

void *doubles(void *params)
{	
	int num;
	parameters* p = (parameters *)params;
	num = p->value1 + p->value2;
	p->returnval = 2*num;
	printf("doubles returns: %d \n",p->returnval);
	pthread_exit(0);
}

/* Thread2 begins execution in this function */

void *triples(void *params)
{	
	int num;
	parameters* p = (parameters *)params;
	num = p->value1 + p->value2;
	p->returnval = 3*num;
	printf("triples returns: %d \n",p->returnval);
	pthread_exit(0);
}

/* Thread3 begins execution in this function */

void *sum(void *params)
{
	int num;
	parameters* p = (parameters *)params;
	num = p->value1 + p->value2;
	p->returnval = num;
	printf("Sum returns: %d \n",p->returnval);
	pthread_exit(0);
}

