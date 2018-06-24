
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <math.h>
#include <pthread.h>
#include <semaphore.h>

void* ThreadWork(void* tid);

int int_sqrt(int int_in)
{
	int s = static_cast<int>( ceil( sqrt( static_cast<double>(int_in) ) ) );
	return s;
}

int int_log2(int int_in)
{
	int l = static_cast<int>( ceil( log2( static_cast<double>(int_in) ) ) );
	return l;
}

class Barrier
{
	int wait_for;
	sem_t count;
	sem_t barrier1;
	sem_t barrier2;
	int waiting = 0;

public:
	Barrier() { }
	Barrier(int wait_for_in)
	{
		wait_for = wait_for_in;

		sem_init(&count, 0, 1);
		sem_init(&barrier1, 0, 0);
		sem_init(&barrier2, 0, 1);
	}

	void Wait()
	{
		sem_wait(&count);
		waiting++;
		if (waiting == wait_for)
		{
			sem_wait(&barrier2);
			sem_post(&barrier1);
		}
		sem_post(&count);

		sem_wait(&barrier1);
		sem_post(&barrier1);

		sem_wait(&count);
		waiting--;
		if (waiting == 0)
		{
			sem_wait(&barrier1);
			sem_post(&barrier2);
		}
		sem_post(&count);

		sem_wait(&barrier2);
		sem_post(&barrier2);
	}
};

class ShearsortArray
{
	std::vector<int> data;

public:
	int n;
	Barrier barrier;
	ShearsortArray(std::string input_file)
	{
		// Read from file
		std::ifstream f;
		f.open(input_file);
		std::string input_buffer = "";
		while (f.good())
		{
			f >> input_buffer;
			data.push_back(atoi(input_buffer.c_str()));
		}
		f.close();

		// What is the size?
		int a_size = data.size();
		n = int_sqrt(a_size);
		barrier = Barrier(n);
	}

	int Read(int i, int j)
	{
		return data[i*n + j];
	}

	void Write(int i, int j, int val)
	{
		data[i*n + j] = val;
	}

	void Swap(int i1, int j1, int i2, int j2)
	{
		int temp1 = Read(i1, j1);
		int temp2 = Read(i2, j2);
		Write(i1, j1, temp2);
		Write(i2, j2, temp1);
	}

	void Print()
	{
		for(size_t i = 0; i < n; i++)
		{
			for(size_t j = 0; j < n; j++)
			{
				std::cout << Read(i,j) << " ";
			}
			std::cout << std::endl;
		}
	}

	void SortRow(int i)
	{
		bool row_even = ((i+1)%2 != 0);

		bool changed = true;
		while(changed)
		{
			changed = false;
			for(int j = 0; j < n - 1; j++)
			{
				int j1 = j+1;
				if((row_even && Read(i,j) < Read(i,j1)) || (!row_even && Read(i,j) > Read(i,j1)))
				{
					Swap(i,j,i,j1);
					changed = true;
				}
			}
		}
	}

	void SortCol(int j)
	{
		bool changed = true;
		while(changed)
		{
			changed = false;
			for(int i = 0; i < n - 1; i++)
			{
				int i1 = i+1;
				if(Read(i,j) > Read(i1,j))
				{
					Swap(i,j,i1,j);
					changed = true;
				}
			}
		}
	}

	void MT_Sort()
	{
		// Create threads
		std::vector<pthread_t> threads (n);
		std::vector<int> tids (n);
		for (int t = 0; t < n; t++)
		{
			tids[t] = t;
			pthread_create(&threads[t], NULL, ThreadWork, &tids[t]);
		}

		// Wait for threads to finish
		for (int t = 0; t < n; t++)
		{
			pthread_join(threads[t], NULL);
		}
	}
};

ShearsortArray my_array("input.txt");

void* ThreadWork(void* tid)
{
	int t = *((int*)tid);
	int n_phases = 2*int_log2(my_array.n) + 1;

	for (int p = 0; p < n_phases; p++)
	{
		bool p_even = ((p+1)%2 != 0);
		if (p_even)
		{
			my_array.SortCol(t);
		}
		else
		{
			my_array.SortRow(t);
		}
		my_array.barrier.Wait();
	}
}

int main()
{
	my_array.Print();

	my_array.MT_Sort();

	std::cout << std::endl;
	my_array.Print();
}






