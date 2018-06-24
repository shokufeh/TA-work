/**
* Author: Dr. Neerja Mhaskar
* Course: Operating Systems CS 3SH3, Winter 2018
* Disk Scheduler - Assignment 3.
* The disk scheduler program implements FCFS, SSTF, SCAN, C-SCAN, LOOK and C-LOOK disk
* scheduling algorithms in C. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define LEFT "LEFT"
#define RIGHT "RIGHT"
#define DISKEND 199
#define DISKSTART 0
#define TOTAL_CYLINDERS 200

int totalrequests = 0;
int *requests;
int *sortedarray;

//FCFS algorithm
int FCFS(int initialPosition){
    int position = initialPosition;
    int movement = 0;
    int i;
	printf ("FCFS DISK SCHEDULING ALGORITHM:\n\n");
    for (i = 0; i < totalrequests; i++){
        movement = movement + abs(position - requests[i]);
        position = requests[i];
	if(i < totalrequests-1){
        	printf ("%d, ", position);}
	else{
		printf ("%d \n\n", position);}
    }    
    return movement;
}
//Shortest Seek Time First (SSTF) algorithm

int SSTF(int initialPosition){
	int i, localRequest[totalrequests];	
    int position = initialPosition;
    int closest, movement = 0;
	printf ("SSTF DISK SCHEDULING ALGORITHM \n\n");
	for(i=0;i < totalrequests; i++){
		localRequest[i] = requests[i];
	}
	
	for(i=0;i < totalrequests; i++){
        int j,m, closestIndex = 0;
		//find the closest request to the current position that is not marked as -1
		for (j= 0; j < totalrequests; j++){
			if(localRequest[j] != -1){
		    	closest = abs(position - localRequest[j]);
				closestIndex = j;
				break;
			}
		}
        for (j= 0; j < totalrequests; j++){
            if (localRequest[j] != -1 && (abs(position - localRequest[j]) < closest)){
                closest = abs(position - localRequest[j]);
                closestIndex = j;
	    	}	
        }      
		movement += abs(position - localRequest[closestIndex]);
        position = localRequest[closestIndex];
		localRequest[closestIndex] = -1;
	if(i < totalrequests-1){
        	printf ("%d, ", position);}
	else{
		printf ("%d \n\n", position);}
    }  
    return movement;
}

//SCAN disk scheduling algorithm
int SCAN(int initialPosition, const char * headdirection){
	char * curheaddirection; 
        int i, movement=0, count=0; 
	int fsindex=0, endsindex=0;
	
	if(strcmp(headdirection, RIGHT)==0){
		curheaddirection = RIGHT;
		endsindex = totalrequests-1;
	}
	else {curheaddirection = LEFT;
		endsindex = 0;
	}
	
	//to get the first servicing index in the sorted array
	for (i=0; i < totalrequests; i++){
		if (initialPosition < sortedarray[i]){ //if intial position occurs two times, then the second one is choosen
			if(strcmp(curheaddirection, RIGHT)==0){fsindex = i;}
			else{fsindex = i-1;}
			break;							
		}	
	}
	printf ("SCAN DISK SCHEDULING ALGORITHM \n\n");
	while(strcmp(curheaddirection, "")!=0){
		if(strcmp(curheaddirection, RIGHT)==0){
			for(i=fsindex;i<= endsindex;i++){
				movement += abs(initialPosition - sortedarray[i]);						
				if(count < totalrequests-1){
					printf ("%d, ", sortedarray[i]);}
				else{
					printf ("%d \n\n", sortedarray[i]);}
				initialPosition = sortedarray[i];
				count = count+1;				
			}						
			if(count < totalrequests){
				movement += DISKEND-initialPosition;
				endsindex = 0;
				fsindex = fsindex - 1;			
				initialPosition = DISKEND;
				curheaddirection = LEFT;
			}
			else{
				curheaddirection = "";			
			}
		}
		else if(strcmp(curheaddirection, LEFT)==0){
			for(i=fsindex;i>=endsindex;i--){
				movement += abs(initialPosition - sortedarray[i]);			
				if(count < totalrequests-1){
					printf ("%d, ", sortedarray[i]);}
				else{
					printf ("%d \n\n", sortedarray[i]);}
				initialPosition = sortedarray[i];
				count = count+1;					
			}						
			if(count < totalrequests){
				movement += initialPosition;
				endsindex = totalrequests-1;
				fsindex = fsindex + 1;			
				initialPosition = DISKSTART;
				curheaddirection = RIGHT;
			}
			else{
				curheaddirection = "";			
			}
		}
	}	
	return movement;
}

//CSCAN disk scheduling algorithm
int CSCAN(int initialPosition, const char * headdirection){
	char * curheaddirection; 
        int i, movement=0, count=0; 
	int fsindex=0, endsindex=0;
	
	if(strcmp(headdirection, RIGHT)==0){
		curheaddirection = RIGHT;
		endsindex = totalrequests-1;
	}
	else {curheaddirection = LEFT;
		endsindex = 0;
	}
	
	//to get the first servicing index in the sorted array
	for (i=0; i < totalrequests; i++){
		if (initialPosition < sortedarray[i]){ //if intial position occurs two times, then the second one is choosen
			if(strcmp(curheaddirection, RIGHT)==0){fsindex = i;}
			else{fsindex = i-1;}
			break;							
		}	
	}
	printf ("C-SCAN DISK SCHEDULING ALGORITHM \n\n");
	while(strcmp(curheaddirection, "")!=0){
		if(strcmp(curheaddirection, RIGHT)==0){
			for(i=fsindex;i<= endsindex;i++){
				movement += abs(initialPosition - sortedarray[i]);				
				if(count < totalrequests-1){
					printf ("%d, ", sortedarray[i]);}
				else{
					printf ("%d \n\n", sortedarray[i]);}
				initialPosition = sortedarray[i];
				count = count+1;				
			}						
			if(count < totalrequests){
				movement += DISKEND-initialPosition + TOTAL_CYLINDERS-1;
				endsindex = fsindex - 1;
				fsindex = 0;			
				initialPosition = DISKSTART;
			}
			else{
				curheaddirection = "";			
			}
		}
		else if(strcmp(curheaddirection, LEFT)==0){
			for(i=fsindex;i>=endsindex;i--){
				movement += abs(initialPosition - sortedarray[i]);			
				if(count < totalrequests-1){
					printf ("%d, ", sortedarray[i]);}
				else{
					printf ("%d \n\n", sortedarray[i]);}
				initialPosition = sortedarray[i];
				count = count+1;					
			}						
			if(count < totalrequests){
				movement += initialPosition + TOTAL_CYLINDERS-1;
				endsindex = fsindex + 1;
				fsindex = totalrequests-1;			
				initialPosition = DISKEND;
			}
			else{
				curheaddirection = "";			
			}
		}
	}	
	return movement;
}

//LOOK disk scheduling algorithm
int LOOK(int initialPosition, const char * headdirection){
	char * curheaddirection; 
        int i, movement=0, count=0; 
	int fsindex=0, endsindex=0;
	
	if(strcmp(headdirection, RIGHT)==0){
		curheaddirection = RIGHT;
		endsindex = totalrequests-1;
	}
	else {curheaddirection = LEFT;
		endsindex = 0;
	}
	
	//to get the first servicing index in the sorted array
	for (i=0; i < totalrequests; i++){
		if (initialPosition < sortedarray[i]){ //if intial position occurs two times, then the second one is choosen
			if(strcmp(curheaddirection, RIGHT)==0){fsindex = i;}
			else{fsindex = i-1;}
			break;							
		}	
	}
	printf ("LOOK DISK SCHEDULING ALGORITHM \n\n");
	while(strcmp(curheaddirection, "")!=0){
		if(strcmp(curheaddirection, RIGHT)==0){
			for(i=fsindex;i<= endsindex;i++){
				movement += abs(initialPosition - sortedarray[i]);							
				if(count < totalrequests-1){
					printf ("%d, ", sortedarray[i]);}
				else{
					printf ("%d \n\n", sortedarray[i]);}
				initialPosition = sortedarray[i];
				count = count+1;				
			}					
			if(count < totalrequests){
				endsindex = 0;
				fsindex = fsindex - 1;			
				curheaddirection = LEFT;
			}
			else{
				curheaddirection = "";			
			}
		}
		else if(strcmp(curheaddirection, LEFT)==0){
			for(i=fsindex;i>=endsindex;i--){

				movement += abs(initialPosition - sortedarray[i]);			
				if(count < totalrequests-1){
					printf ("%d, ", sortedarray[i]);}
				else{
					printf ("%d \n\n", sortedarray[i]);}
				initialPosition = sortedarray[i];
				count = count+1;					
			}						
			if(count < totalrequests){
				endsindex = totalrequests-1;
				fsindex = fsindex + 1;			
				curheaddirection = RIGHT;
			}
			else{
				curheaddirection = "";			
			}
		}
	}	
	return movement;
}



//CLOOK disk scheduling algorithm
int CLOOK(int initialPosition, const char * headdirection){
	char * curheaddirection; 
        int i, movement=0, count=0; 
	int fsindex=0, endsindex=0;

	if(strcmp(headdirection, RIGHT)==0){
		curheaddirection = RIGHT;
		endsindex = totalrequests-1;
	}
	else {curheaddirection = LEFT;
		endsindex = 0;
	}

	//to get the first servicing index in the sorted array
	for (i=0; i < totalrequests; i++){
		if (initialPosition < sortedarray[i]){ //if intial position occurs two times, then the second one is choosen
			if(strcmp(curheaddirection, RIGHT)==0){fsindex = i;}
			else{fsindex = i-1;}
			break;							
		}	
	}
	printf ("C-LOOK DISK SCHEDULING ALGORITHM \n\n");
	while(strcmp(curheaddirection, "")!=0){
		if(strcmp(curheaddirection, RIGHT)==0){
			for(i=fsindex;i<= endsindex;i++){
				movement += abs(initialPosition - sortedarray[i]);							
				if(count < totalrequests-1){
					printf ("%d, ", sortedarray[i]);}
				else{
					printf ("%d \n\n", sortedarray[i]);}
				initialPosition = sortedarray[i];
				count = count+1;				
			}						
			if(count < totalrequests){
				movement += sortedarray[totalrequests-1]- sortedarray[0];
				endsindex = fsindex - 1;
				fsindex = 0;			
				initialPosition = sortedarray[fsindex];
				//printf("initialposition=%d\n", initialPosition);
			}
			else{
				curheaddirection = "";			
			}
		}
		else if(strcmp(curheaddirection, LEFT)==0){
			for(i=fsindex;i>=endsindex;i--){
				movement += abs(initialPosition - sortedarray[i]);			
				if(count < totalrequests-1){
					printf ("%d, ", sortedarray[i]);}
				else{
					printf ("%d \n\n", sortedarray[i]);}
				initialPosition = sortedarray[i];
				count = count+1;					
			}						
			if(count < totalrequests){
				movement += sortedarray[totalrequests-1]- sortedarray[0];
				endsindex = fsindex + 1;
				fsindex = totalrequests-1;			
				initialPosition = sortedarray[fsindex];
				//printf("initialposition=%d\n", initialPosition);
			}
			else{
				curheaddirection = "";			
			}
		}
	}	
	return movement;
}

//Sorting the request using merge sort.
sortArray(int * array, int from, int to, int size){ //sort array using merge sort.
	
	int temp;
	if(size==1){//do nothing
		return 1;
	}
	else {
		sortArray(array, from, from+(size/2)-1, size/2);
		sortArray(array, to+1-(size/2 + (size%2)), to, size/2 +(size%2));
		merge(array, from,to+1-(size/2 + (size%2)),size/2 ,size/2 +(size%2));
	}
	return 1;
}

int merge (int * array, int index1, int index2, int size1, int size2) {
	int i=0,j=0;
	int temp[size1+size2];
	if(size1==1 && size2==1){
		if (array[index1] > array[index2]){
			int temp = array[index1];
			array[index1] = array[index2];
			array[index2]=temp;

		}
		return 1;
	}
	while(i < size1 || j < size2){
		if(i < size1 && j < size2){
			if(array[index1+i] <= array[index2+j]) {
				temp[i+j]= array[index1+i];
				i=i+1;	
			}
			else{
				temp[i+j]= array[index2+j];
				j=j+1;	
			}
		}
		else{
			//if all elements from requests i..i+size1-1 are merged, merge 
		// the remaining elements of request from j..j+size2-1 
			if(i==size1){//checking if remaining elements in other list
				temp[i+j]= array[index2+j];
				j++;
			}
			//if all elements from requests j..j+size2-1 are merged, merge 
			// the remaining elements of request from i..i+size1-1 
			if(j==size2){//checking if remaining elements in other list
				temp[i+j]= array[index1+i];
				i++;
			}
		}
	}
	for(i=0;i<size1+size2;i++){
		array[index1+i]=temp[i];
	}
}

int main(int argc, const char *argv[]){

	/*Print Usage*/
	if (argc != 3) {
		fprintf(stderr,"Usage: ./a.out <INITIAL POSITION> <HEAD DIRECTION>\n");
        
		return -1;
	}
	int initialPosition = atoi(argv[1]); 
	/*Read the file*/
	FILE *fp = fopen("request1.bin", "rb");
	int i, n=0;
	if(fp == NULL){
		fprintf(stderr,"Could not open file: request.bin.\n");
      		return -1;
	}
	fseek(fp,0L,SEEK_END);
   	n = ftell(fp);
	n = n/4;
	rewind(fp);
	totalrequests=n;
   	printf("Total requests =%d \n", n);
	requests = (int *)malloc(sizeof(int)*n);
	sortedarray = (int *)malloc(sizeof(int)*n);
	fread(requests, sizeof(int),n,fp);
	/*for(i=0;i< n;i++){ 
		printf("requests[%d]=%d\n", i, requests[i]);
	}*/
	fclose(fp);
    	for(i=0;i< totalrequests;i++){
		sortedarray[i] = requests[i];
    	}
    	sortArray(sortedarray, 0,totalrequests-1,totalrequests);	//sort the array sorted array	
      
    	printf("FCFS - Total head movements = %d \n\n", FCFS(initialPosition));
    	printf("SSTF - Total head movements = %d \n\n", SSTF(initialPosition));
   	printf("SCAN - Total head movements = %d \n\n", SCAN(initialPosition, argv[2]));
	printf("C-SCAN - Total head movements = %d \n\n", CSCAN(initialPosition, argv[2]));
    	printf("LOOK - Total head movements = %d \n\n", LOOK(initialPosition, argv[2]));
    	printf("C-LOOK - Total head movements = %d \n\n", CLOOK(initialPosition, argv[2]));
	return 0;	
}
