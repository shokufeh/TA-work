/**
 * Author: Dr. Neerja Mhaskar
 * Course: Operating Systems Comp Sci 3SH3, Winter 2018
 * Solution to lab test1
 */

#include <stdio.h>
#include  <sys/types.h> /* This header file has the definition for pid_t type*/

int main()
{
    printf("%d\n",getpid());
	pid_t pid1;	
	pid1 = fork();
     printf("%d\n",getpid());
    
   
    
    
	if(pid1 == 0) {	
		printf("%d\n",getpid());
		pid_t pid2 = fork();
		if(pid2==0){	
			pid_t pid3 = fork();
			if(pid3==0){
				pid_t pid4 = fork();
					if(pid4==0){				
						printf("%d\n",getpid());}
					else{
						printf("%d\n",getpid());
						wait();}	
			}			
			else{
				printf("%d\n",getpid());				
				wait();
			}
		}
		else{
			wait();
		}
	}
	else{
		printf("%d\n",getpid());
        
		wait();}
	return 0;
}
