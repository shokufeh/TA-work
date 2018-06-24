#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
//Justin Hole 400012906
//Adrian Paluch 400036453
//Lab Group 17

int main()
{
	printf("%d\n",getpid()); //output node 1
	pid_t pid = fork(); //node1 creates child node2
	
	if(pid == 0) { //if I am node2
		printf("%d\n",getpid()); //output node 2
		pid_t pid1 = fork(); //node2 forks creates node3

		if (pid1 == 0) { //if I am node3
			printf("%d\n",getpid()); //output node 3
			pid_t pid2 = fork(); //node3 creates node4
			
			if (pid2 == 0) { //if I am I node 4
				printf("%d\n",getpid()); //output node4
				pid_t pid3 = fork(); //node4 creates node5
			
				if (pid3 == 0){ //if I am node 5
					printf("%d\n",getpid()); //output node5
					wait(); //node 5 waits
				}
				else { wait();} //node 4 waits
			}
			else { wait();} //node 3 waits
		}
		else { wait();} //node2 waits
	}
	else { wait();} //node1 waits
	return 0;
}
