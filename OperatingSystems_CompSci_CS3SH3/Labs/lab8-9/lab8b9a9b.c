/**
* Author: Dr. Neerja Mhaskar
* Course: Operating Systems CS 3SH3, Winter 2018
* Solution for Practice labs 8b, 9a, 9b.
*/

#include <stdio.h>
#include <sys/mman.h> /*For mmap() function*/
#include <string.h> /*For memcpy function*/
#include <fcntl.h> /*For file descriptors*/
#include <stdlib.h> /*For file descriptors*/

#define	INT_SIZE 4
#define INT_COUNT 10

#define MEMORY_SIZE INT_COUNT * INT_SIZE

// Pointer to memory mapped backing file
char *mmapfptr;
//Character array to store characters from memory backed file.
int intArray[INT_COUNT];

int main(int argc, const char *argv[])
{   
    //Returns a file descriptor for the file entered on command line.
    int mmapfile_fd = open("numbers.bin", O_RDONLY); 
    //Pointer that stores the starting address of the memory mapped file.
    mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);
    
    
    
    char value; //value of the character read from the memory mapped file and saved in buffer.
    
    int i, sum = 0;
    for(i=0;i< INT_COUNT;i++){	                     
		// Copy characters (numbers) from memory mapped file to character array.
		memcpy(intArray + i, mmapfptr + i*INT_SIZE, INT_SIZE);
    } 	
    for(i=0;i< INT_COUNT;i++){	   
		printf("array value %d \n",intArray[i]);                  
		sum = sum + intArray[i];  
    } 	
    printf("Sum of numbers = %d \n", sum);         		
    munmap(mmapfptr, MEMORY_SIZE); 	//unmap the memory file
    return 0;
}
