#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

int main(void){
        int     fd[2], nbytes;
        int     fd1[2];
        pid_t   childpid;
        char    string[] = "We love 3SH3!\n";
        char    readbuffer[80];

        pipe(fd);
        pipe(fd1);
        
        if((childpid = fork()) == -1){
                perror("fork");
                exit(0);
        }

        if(childpid == 0){

            int i=0;

            close(fd[0]);
            close(fd1[1]);
		
            cout << endl;
            while(true){
                cout << "Enter a 1-byte integer: ";
                cin >> i;
  //              printf("child: %d\n", i);
                write(fd[1], &i, 1);
                if (i == -1){
                        read(fd1[0], readbuffer, 1);
                        cout << endl;
                        printf("child: The sum is: %d\n", readbuffer[0]);
                        cout << endl;
                        break;
                }
            }
        }
        else {
                close(fd[1]);
                close(fd1[0]);
		
                int sum = 0;
                while(true){
                    nbytes = read(fd[0], readbuffer, 1);
    //                printf("parent: %d\n", readbuffer[0]);
                    if ((int)(readbuffer[0]) != -1){
                        sum += readbuffer[0];
//                        printf("parent: %d\n",  sum);
				
                    }
                    else write(fd1[1], &sum, 1);
                }
        }
        return 0;
}
