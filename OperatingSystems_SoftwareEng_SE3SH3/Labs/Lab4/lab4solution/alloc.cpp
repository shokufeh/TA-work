
/*
	Implements a resource allocator program
	Opens res.txt and maps it to a memory region using the system call mmap()
	Make sure the size of the region is not smaller than the filesize using fstat()
	In a loop, keeps asking how many resources of a resource type are needed
	Subtracts the units from the resource type if available and then uses msync() to sync the changes to the file on disk
*/

#include <iostream>
#include <unistd.h>

#include "easymmap.h"

int main ()
{
	// Create mapped memory region
	char* memory_map;
	int file = open("res.txt", O_RDWR | O_SYNC, S_IWRITE | S_IREAD);
	bool sem_create = true;
	easymmap_init(file, memory_map, sem_create);

	// Main loop
	while(true)
	{
		// Are resources needed?
		std::cout << "Are any resources needed? (y/N): ";
		char needed;
		std::cin >> needed;

		if (needed == 'y')
		{
			// Ask user what type and how many resources are needed
			std::cout << "What type? ";
			char res_type;
			std::cin >> res_type;

			std::cout << "How many? ";
			char res_amount;
			std::cin >> res_amount;

			// Update resources in mmapped file
			int amount_added = -1 * (res_amount - '0');
			easymmap_update_resource(res_type, amount_added, file, memory_map);
		}
	}

	// Destroy mapped memory region
	easymmap_destroy(file, memory_map);
	close(file);

	return 0;
}