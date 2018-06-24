
/*
	Parent process
		Opens res.txt and maps it to a memory region using the system call mmap() before forking the child process
		Acts as a provider of resources
		In a loop, keeps asking wether new resources need to be added
		If yes, get user input (resource type + num of units) and adds to the memory region
		Use msync() to save changes to physical file
	Child process
		Reports every 10 seconds:
			page size of the system (getpagesize())
			current state of resources
			current status of pages in the memory region (mincore())
*/

#include <iostream>
#include <unistd.h>

#include "easymmap.h"


int main ()
{
	// Create mapped memory region
	char* memory_map;
	int file = open("res.txt", O_RDWR | O_SYNC, S_IWRITE | S_IREAD);
	bool sem_create = false;
	easymmap_init(file, memory_map, sem_create);

	// Fork
	pid_t pid = fork();
	if (pid == 0)
	{
		// Child
		while (true)
		{
			int file_size = easymmap_get_file_size(file);
			int page_size = getpagesize();
			int vec_len = (file_size + page_size - 1) / page_size;
			unsigned char* page_status = (unsigned char*)malloc(vec_len);
			bool resident = (page_status[0] & 1) == 1;
			std::cout << "Page size: " << page_size << std::endl;
			std::cout << "Resources: " << std::endl << memory_map << std::endl;
			if (resident)
			{
				std::cout << "Page status: Resident" << std::endl;
			}
			else
			{
				std::cout << "Page status: Not resident" << std::endl;
			}
			free(page_status);
			sleep(10);
		}
	}
	else
	{
		// Parent
		while(true)
		{
			// Are resources needed?
			std::cout << "Are there any resources that need to be added? (y/N): ";
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
				int amount_added = res_amount - '0';
				easymmap_update_resource(res_type, amount_added, file, memory_map);
			}
		}
	}

	return 0;
}