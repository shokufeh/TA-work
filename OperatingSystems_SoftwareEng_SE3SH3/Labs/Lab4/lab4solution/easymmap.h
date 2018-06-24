
#ifndef EASYMMAP_H
#define EASYMMAP_H

#include <iostream>
#include <vector>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

// Semaphore setup
#define SEMAPHORE_NAME "easymmap_sem"
sem_t* easymmap_sem;

// State machine enums
#define READSTATE_RESTYPE (0)
#define READSTATE_SPACE (1)
#define READSTATE_RESAMOUNT (2)
#define READSTATE_NEWLINE (3)
#define SEARCHSTATE_LOOKING (0)
#define SEARCHSTATE_FOUND (1)
#define SEARCHSTATE_DONE (2)

int easymmap_get_file_size(int file_descriptor)
{
	struct stat file_stats;
	fstat(file_descriptor, &file_stats);
	return (int)file_stats.st_size;
}

void easymmap_init(int file_descriptor, char* &memory_map, bool sem_create)
{
	// Map file to memory
	int file_size = easymmap_get_file_size(file_descriptor);
	memory_map = (char*)mmap(NULL, file_size, PROT_READ|PROT_WRITE, MAP_SHARED, file_descriptor, 0);
	if (memory_map == MAP_FAILED)
	{
		close(file_descriptor);
		std::cerr << "Memory mapping failed" << std::endl;
		exit(1);
	}

	// Initialize semaphore
	if (sem_create)
	{		
		easymmap_sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0644, 0);
		sem_post(easymmap_sem);
	}
	else
	{
		easymmap_sem = sem_open(SEMAPHORE_NAME, 0);
	}
}

void easymmap_destroy(int file_descriptor, char* &memory_map)
{
	// Unmap the memory region
	int file_size = easymmap_get_file_size(file_descriptor);
	munmap(memory_map, file_size);

	// Destroy semaphore
	sem_close(easymmap_sem);
}

void easymmap_mutex_lock()
{
	sem_wait(easymmap_sem);
}

void easymmap_mutex_unlock()
{
	sem_post(easymmap_sem);
}

int easymmap_update_resource(char res_type, int res_amount_add, int file_descriptor, char* &memory_map)
{
	easymmap_mutex_lock();

	int read_state = READSTATE_RESTYPE;
	int search_state = SEARCHSTATE_LOOKING;
	int file_size = easymmap_get_file_size(file_descriptor);

	for (size_t i = 0; i < file_size; i++)
	{
		char c = memory_map[i];
		int next_state;

		switch(read_state)
		{
			case READSTATE_RESTYPE:
				// Did we find the type?
				if (c == res_type)
				{
					search_state = SEARCHSTATE_FOUND;
				}
				else
				{
					search_state = SEARCHSTATE_LOOKING;
				}
				next_state = READSTATE_SPACE;
				break;
			case READSTATE_SPACE:
				// Skip this character
				next_state = READSTATE_RESAMOUNT;
				break;
			case READSTATE_RESAMOUNT:
				// If this is the one to be updated, do the update and finish
				if (search_state == SEARCHSTATE_FOUND)
				{
					// Get char to write to file
					int amount_found = c - '0';
					int new_amount = amount_found + res_amount_add;
					if (new_amount >= 0)
					{					
						char new_char = (char)new_amount + '0';
						memory_map[i] = new_char;
					}
					else
					{
						std::cout << "Not enough resources to allocate" << std::endl;
					}
					
					// Signal we're done
					search_state = SEARCHSTATE_DONE;
				}
				next_state = READSTATE_NEWLINE;
				break;
			case READSTATE_NEWLINE:
				// Skip this character
				next_state = READSTATE_RESTYPE;
				break;
		}
		read_state = next_state;
	}

	// Sync changes to physical file
	msync(memory_map, (size_t)file_size, MS_SYNC);

	easymmap_mutex_unlock();
}

#endif
