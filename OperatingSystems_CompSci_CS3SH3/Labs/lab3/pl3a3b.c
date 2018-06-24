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

#define NUMThreads 2
#define SIZE 20
//int list[SIZE] = {77,12,19,43,18,24,21,62,15,18,11,16,9,12,18, 7,33,22, 1, 2};
int list[SIZE] = {1, 2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
void *sum(void *param);

/*Parameter data structure created to store a range in the list.*/
typedef struct threadparam {
	int from_index;
	int to_index;
	int returnval;
}parameters;

int main(void)
{
	int i;
    	int total=0;
	pthread_t workers[NUMThreads];
	pthread_attr_t attr;
	 
    
	
    
    
    /* establish the first summing thread */
    parameters *data = (parameters *) malloc (sizeof(parameters));
    data->from_index = 0;
    data->to_index = (SIZE/2) - 1;
    data->returnval = 0;
    
    
    /* establish the second summing thread */
    parameters *data1 = (parameters *) malloc (sizeof(parameters));
    data1->from_index = (SIZE/2);
    data1->to_index = SIZE - 1;
    data1->returnval = 0;
    

    
    pthread_create(&workers[0], 0 , sum, data);
    pthread_create(&workers[1], 0, sum, data1);

    
    
   	/* now wait for the 2 summing threads to finish */
    for (i = 0; i < NUMThreads - 1; i++)
        pthread_join(workers[i], NULL);
	/* output the sum */
	
    total = data->returnval + data1->returnval;
	printf("Sum of numbers in the list is:%d \n",total);
	free(data);
	free(data1);
    return 0;
}

/**
 *Summing thread.
 *
 */

void *sum(void *params)
{
	int i;
	parameters *p = (parameters *)params;
	int begin = p->from_index;
	int end = p->to_index;
	int large = p->returnval;
	int sum=0;
	printf("Begining: %d, and end: %d \n", begin, end);
	
	for (i = begin; i <= end; i++) {
		sum = sum + list[i];
	} 
	p->returnval = sum;
	pthread_exit(0);
}

