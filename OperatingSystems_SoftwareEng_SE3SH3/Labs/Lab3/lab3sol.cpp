#include <iostream>
#include <fstream>
#include <stdint.h>
#include <pthread.h>

using namespace std;

#define N 4

int array[N][N];
int phase = 1;
int counter = 0;

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t count_cv = PTHREAD_COND_INITIALIZER;


void print()
{
    if (phase == 0) cout << endl << endl << "INPUT ARRAY:" << endl;
    else cout << "PHASE " << phase << endl;
    cout << "--------------------" << endl;
    for (int i = 0; i < N; i++){
        for(int j = 0; j < N; j++) {
            cout << array[i][j] << " ";
        }
        cout << endl;
    }
    
    cout << endl;
}

void *sort(void* tid)
{
    int*  data = reinterpret_cast<int*>(tid);
    int   id    = *data;
    
    // Bubble sort

    while(true){
        if(phase % 2 == 0){ // For columns
            for(int i = 0; i <= N; i++){
                for(int j = 0; j < N-1; j++) {
                    if(array[j][id] > array[j+1][id]) {
                        int temp = array[j][id];
                        array[j][id] = array[j+1][id];
                        array[j+1][id] = temp;
                    }
                }
            }
        }
        else {
            if (id % 2 == 0){ // For even rows
                for(int i = 0; i <= N; i++){
                    for(int j = 0; j < N-1; j++) {
                        if(array[id][j] > array[id][j+1]) {
                            int temp = array[id][j];
                            array[id][j] = array[id][j+1];
                            array[id][j+1] = temp;
                        }
                    }
                }
            }
            else { // For odd rows
                for(int i = 0; i <= N; i++){
                    for(int j = 0; j < N-1; j++) {
                        if(array[id][j] < array[id][j+1]) {
                            int temp = array[id][j];
                            array[id][j] = array[id][j+1];
                            array[id][j+1] = temp;
                        }
                    }
                }
            }
        }
        
        if (phase == N+1){ // Exit thread in the last phase
            delete data;
            pthread_exit(NULL);
        }
        
        // Wait for signal to go the next phase
        
        pthread_mutex_lock(&count_mutex);
        counter++;
        pthread_cond_wait(&count_cv, &count_mutex);
        pthread_mutex_unlock(&count_mutex);
    }
    
}

int main () {
    
    pthread_t threads[N];
    
    // Reading input from file
    
    ifstream myfile;
    myfile.open ("input.txt");
    for (int i = 0; i < N; i++){
        for(int j = 0; j < N; j++) {
            myfile >> array[i][j];
        }
    }
    myfile.close();

    phase = 0;
    print();
    
//    phase = 1;
    
    // Creating threads
    
    for (int i = 0; i < N; i++){
        int rc = pthread_create(&threads[i], NULL, sort, new int(i));
    }
    
    // Synchronizing rach phase: when all threads are done, send a broadcast wake up
    while (true){
        if (phase == N+1)
            break;
        
        pthread_mutex_lock(&count_mutex);
        
        if (counter == N) {
            phase++;
            print();
            counter = 0;
            pthread_cond_broadcast(&count_cv);

        }
        pthread_mutex_unlock(&count_mutex);
        
    }

    // Wait for all threads to finish before terminating the process
    for (int i = 0; i < N; i++)
        pthread_join(threads[i], NULL);
    
    print();
    
    return 0;
}
