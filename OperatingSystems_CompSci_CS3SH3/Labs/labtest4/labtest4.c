/*
*
* Author: Dr. Neerja Mhaskar
* Course: Operating Systems Comp Sci 3SH3, Winter 2018
* Solution to practice labs 7a, 7b and 8a.
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h> /*For mmap() function*/
#include <string.h> /*For memcpy function*/
#include <fcntl.h> /*For file descriptors*/
#include <stdlib.h> /*For file descriptors*/

#define OFFSET_MASK 255			// 2^8 -1
#define PAGES 10
#define OFFSET_BITS  8		//Number of bits to represent an offset = number of bits to 							represent page size.
#define PAGE_SIZE 256	

#define	INT_SIZE 4 //size in bytes
#define MEMORY_SIZE PAGES * INT_SIZE

// Pointer to memory mapped backing file
char *mmapfptr;
//Integer array to simulate page table.
int page_table[PAGES];

int main(int argc, char *argv[]) {
	unsigned int page_number, frame_number;
	int offset;
	unsigned int virtual_address, physical_address;
	char buff[10];

	//Returns a file descriptor for the file entered on command line.
    	int mmapfile_fd = open("page_table.bin", O_RDONLY); 
    	//Pointer that stores the starting address of the memory mapped file.
    	mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0); 

    	int i, sum = 0;
    	for(i=0;i< PAGES;i++){	                     
		// Copy numbers from memory mapped file to integer array.
		memcpy(page_table + i, mmapfptr + i*INT_SIZE, INT_SIZE);	    
    	} 	
	munmap(mmapfptr, MEMORY_SIZE); 	//unmap the memory file
	/*Declare a file pointer fptr. fopen initializes an obect of type file.
	 This is stored in fptr. The mode 'r' indicates that the file is for 
	read only purpose.*/
	FILE *fptr = fopen("ltaddr.txt", "r");

	if (fptr == NULL) {
		printf("Error in opening file \n");
		return -1;
	}
	/*Read 10-1 = 9 characters from the file at a time. 
	These characters are stored in buff*/

	while(fgets(buff, 10, fptr) != NULL) {
		virtual_address = atoi(buff);
		/*Mask the page number, this will enable you get your 
		page number bits, then do a bit shift to the right by the 
		number of offset bits. To get all the page number bits.
		The compiler automatically outputs the correct decimal 
		representation of it.*/
		page_number = virtual_address >> OFFSET_BITS;
		frame_number = page_table[page_number];
		offset = virtual_address & OFFSET_MASK;
		/*Move the bits in frame_number to left by the number of bits in
		the offset bit, then OR it with the offset to get the physical address.*/
		physical_address = (frame_number << OFFSET_BITS) | offset;
		printf("Virtual addr is %d: Page# = %d & Offset = %d frame number = %d Physical addr = %d. \n", 			virtual_address, page_number, offset, frame_number, physical_address);
	}
	/*Close the file.*/
 	fclose(fptr);

	return 0;
}
