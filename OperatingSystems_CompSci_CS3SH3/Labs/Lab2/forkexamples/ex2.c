/**
 * Author: Dr. Neerja Mhaskar
 * Course: Operating Systems CS 3SH3, Winter 2018
 * Example to help you work on lab3a	
 * You can verify your answer by counting 
 * the number of unique processes which are output
 * by the call to getpid() - which is 3 unique processes.
 * Note: The statement 'printf("%d\n",getpid());' prints
 * the pid of the current process.
 * Process tree: The root P1 has two children P2 and P3.
 */


#include <stdio.h>
#include  <sys/types.h> /* This header file has the definition for pid_t type */

int main()

{
	pid_t pid;	
	pid = fork();
 	printf("%d\n",getpid());
    
    if(pid == 0) {
        /* do nothing */
        
    }
    else {
        fork();
        printf("%d\n",getpid());
    }

    
    	
	return 0;
}


