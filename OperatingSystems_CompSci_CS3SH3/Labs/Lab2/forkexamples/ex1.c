/**
 * Author: Dr. Neerja Mhaskar
 * Course: Operating Systems CS 3SH3, Winter 2018
 * Example to help you work on lab2a	
 * You can verify your answer by counting 
 * the number of unique processes which are output
 * by the call to getpid() - which is 5 unique processes.
 * Note: The statement 'printf("%d\n",getpid());' prints
 * the pid of the current process.
 * Process tree: The root P1 has a child P2, P2 has two children P3 and P4, and finally P4 has a child P5.
 */

#include <stdio.h>
#include  <sys/types.h> /* This header file has the definition for pid_t type*/

int main()
{
	pid_t pid;	
	pid = fork();
	printf("%d\n",getpid());
    
    if(pid == 0) {
        pid_t pid1=fork();
        if(pid1 == 0) {
            printf("%d\n",getpid());}
        else
        {wait();}
        pid_t pid2=fork();
        if(pid2 == 0) {
            printf("%d\n",getpid());}
        else
        {wait();}
    }
    
    else
    {wait();}
    
	return 0;

}

