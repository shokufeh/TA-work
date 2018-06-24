#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
// user-defined signal handler for alarm.
void alarm_handler(int signo){
if (signo == SIGALRM){
    printf("Alarm\n");
    }
}

void C_handler(int signo){

if (signo == SIGINT){
        printf("CTRL+C pressed!\n");
    }

}

void Z_handler(int signo){

if (signo == SIGTSTP){
        printf("CTRL+Z pressed!\n");
        exit(1);
       // kill();
    }
}






int main(void){
    // register the signal handler
    if (signal(SIGALRM , alarm_handler) == SIG_ERR){

        printf("failed to register alarm handler.");
        exit(1);
    }
    if (signal(SIGINT , C_handler) == SIG_ERR){

        printf("failed to register C handler.");
        exit(1);
    }
    if (signal(SIGTSTP , Z_handler) == SIG_ERR){

        printf("failed to register Z handler.");
        exit(1);
    }
    //signal(SIGALRM, SIG IGN);


   // alarm(2); // set alarm to fire in 3 seconds.
    while(1){
       // sleep(1);
        alarm_handler(int signo);
        Z_handler(int signo);
        C_handler(int signo);
        alarm(2);
        sleep(2);
    } // wait until alarm goes off

}
