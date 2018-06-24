//
//  main.cpp
//  Lab4
//
//  Created by Borzoo Bonakdarpour on 2016-03-07.
//  Copyright © 2016 Borzoo Bonakdarpour. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <sys/ipc.h>
#include <sys/sem.h>

using namespace std;

#define SIZE 10

#define KEY (1976)

static void* res;
struct stat st;

union semun {
    int val;
    struct semid_ds *buf;
    ushort * array;
} argument;

struct sembuf operations[1];

int id;


void Reporter(){
    while(true){
        
        char* resource = (char*)res;
        sleep(10);
        cout << endl << "Report:" << endl;
        
        cout << "Page size is: " << getpagesize() << endl << endl;
        cout << "Number of pages needed:" << (st.st_size / getpagesize()) + 1 << endl << endl;
        cout << "Current state of resources:" << endl << endl;

        //wait
        operations[0].sem_num = 0;
        operations[0].sem_op = -1;
        operations[0].sem_flg = 0;
        
        int retval = semop(id, operations, 1);
        
        if(retval == 0){}
        else {
            printf("P-operation did not succeed.\n");
        }
        
        cout << resource;
        
        //signal
        operations[0].sem_num = 0;
        operations[0].sem_op = 1;
        operations[0].sem_flg = 0;
        retval = semop(id, operations, 1);
        
    
        unsigned char vec[SIZE];
        
        if (mincore(res, SIZE, &vec[0]) != -1){
            cout << "Status of pages in main memory: " << endl;
            for (int i = 0; i < (st.st_size / getpagesize()) + 1; i++){
                cout << "Page " << i << ": ";
                if ((unsigned short int)(vec[i] & 1) == 1)
                    cout << "Resident";
                else
                    cout << "Non-resident";
                    
                    cout << endl;
            }
            cout << endl;
        }

    }
}

void Provider(){

    while(true){
        char answer;
        
        cout << "Provide more resources(y/n)? ";
        cin >> answer;
        if (answer == 'n') {
            exit(0);
        }
        else{
            char* resource = (char*)res;

            int resNo, count;
            cout << "Enter the resource number and number of additional resources to be provided: ";
            cin >> resNo >> count;
            
            for (int i = 0; i <= st.st_size; i += 4){
                
                //wait
                operations[0].sem_num = 0;
                operations[0].sem_op = -1;
                operations[0].sem_flg = 0;
                
                int retval = semop(id, operations, 1);
                
                if(retval == 0){}
                else {
                    printf("P-operation did not succeed.\n");
                }
                
                if(resource[i] == resNo + 48 && resource[i+2] - 48 + count <= 9){
                    resource[i+2] += count;

                    //signal
                    operations[0].sem_num = 0;
                    operations[0].sem_op = 1;
                    operations[0].sem_flg = 0;
                    retval = semop(id, operations, 1);
                    if (msync(res, SIZE, MS_ASYNC) != -1)
                        cout << "Synced successfully." << endl;
                    break;
                }
                
                //signal
                operations[0].sem_num = 0;
                operations[0].sem_op = 1;
                operations[0].sem_flg = 0;
                retval = semop(id, operations, 1);
            }

        }
    }
}


int main() {

    pid_t   childpid;

    int fd = -1;

    //Semaphore initialization
    id = semget(KEY, 1, 0666 | IPC_CREAT);
    
    if(id < 0) {
        fprintf(stderr, "Unable to obtain semaphore.\n");
        exit(0);
    }
    
    if( semctl(id, 0, SETVAL, argument) < 0) {
        fprintf( stderr, "Cannot set semaphore value.\n");
    }
    else {
        fprintf(stderr, "Semaphore %d initialized.\n", KEY);
    }
    //Initial value of the semaphore is 1
    operations[0].sem_num = 0;
    operations[0].sem_op = 1;
    operations[0].sem_flg = 0;
    int retval = semop(id, operations, 1);
    
    if(retval == 0) {}
    else {
        printf(" V-operation did not succeed.\n");
        perror("REASON");
    }

    
    fd = open("res.txt", O_RDWR);

//    fd = open("/Users/borzoo/Borzoo/Teaching/17/3SH3/Labs/Lab4//Lab4/res.txt", O_RDWR);
//    fd = open("/Volumes/borzoo/Borzoo/Teaching/17/3SH3/Labs/Lab4/res.txt", O_RDWR);

    
    if (fd == -1){
        cout << "File not found." << endl << endl;
        return 0;
    }
    
    if (0 != fstat(fd, &st)) {
        perror("fstat");
        close(fd);
        return 0;
    }
    
    res = mmap(NULL, st.st_size + 1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    if (res == (void*) MAP_FAILED){
        close(fd);
        return 0;
    }
    
    if((childpid = fork()) == -1){
        perror("fork");
        return 0;
    }
    
    if(childpid == 0){ // Reporter
        Reporter();

    }
    else { // Provider
        Provider();
    }

    munmap(res, SIZE);
    close(fd);
    return 0;
}
