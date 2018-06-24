#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>

 void alarm_handler(int signo) {
     if (signo == SIGALRM){
         printf("Alarm!\n");
     }
 }

 void ctrlc_handler(int signo) {
     if (signo == SIGINT){
         printf("CTRL+C Pressed!\n");
     }
 }

 void ctrlz_handler(int signo) {
     if (signo == SIGTSTP){
         printf("CTRL+Z Pressed!\n");
	 exit(0);
     }
 }

 int main(void) {
     // register the signal handler
     if (signal(SIGALRM, alarm_handler) == SIG_ERR){

         printf("failed to register alarm handler.");
         exit(1);
     }

     if (signal(SIGINT, ctrlc_handler) == SIG_ERR){

         printf("failed to register alarm handler.");
         exit(1);
     }

     if (signal(SIGTSTP, ctrlz_handler) == SIG_ERR){

         printf("failed to register alarm handler.");
         exit(1);
     }
  
     while(1){
	 sleep(2);
	 raise(SIGALRM);
     //or it can be as follows
	 // raise(SIGALARM);
	 //sleep(2);
	 //which means the order does not matter
	 //also you can put alarm(2) instead of raise(SIGALARM) 
	 //the idea is you can either call alarm or send alarm signal to the process
	 
	 
	 
	 }
 }
