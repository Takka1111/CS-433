/**
* Assignment 4: Producer Consumer Problem
 * @file main.cpp
 * @author Abraham Gomez and Tucker Shaw
 * @brief The main program for the producer consumer problem.
 * @version 0.1
 */

#include "buffer.h"
#include <iostream>
#include <unistd.h>
#include <pthread.h>    // For pthread_mutex
#include <semaphore.h>  // For sem_t

using namespace std;

// Declare global variables
Buffer buffer;          // Global buffer object
pthread_mutex_t mutex_; // The mutex lock
sem_t full_;            // Semaphore for full buffer
sem_t empty_;           // Semaphore for empty buffer

/**
 * @brief Producer function
 * @param param The ID for the buffer item
 */
void *producer(void *param) {
    buffer_item item = *((int *) param); // Produce an item with unique ID

    // Thread will continue to loop until main exits
    while (true) {
        usleep(rand() % 1000000); // Sleep for a random period of time

        sem_wait(&empty_);              // Call wait on the empty_ semaphore
        pthread_mutex_lock(&mutex_);    // Acquire the mutex_ lock

        if (buffer.insert_item(item)) {
            cout << "Producer " << item << ": Inserted item " << item << endl;

            buffer.print_buffer(); // Print the buffer contents
        } else 
            cout << "Producer error condition"  << endl;    // Error (should not execute)

        pthread_mutex_unlock(&mutex_);  // Release the mutex_ lock
        sem_post(&full_);               // Call post on the full_ semaphore
    }
}

/**
 * @brief Consumer function
 * @param param The ID for the buffer item
 */
void *consumer(void *param) {
    buffer_item item; // Create a buffer item container

    // Thread will continue to loop until main exits
    while (true) {
        usleep(rand() % 1000000); // Sleep for a random period of time

        sem_wait(&full_);               // Call wait on the full_ semaphore
        pthread_mutex_lock(&mutex_);    // Acquire the mutex_ lock

        // Consume an item and print result
        if (buffer.remove_item(&item)) {
            cout << "Consumer Removed item " << item << endl;

            buffer.print_buffer(); // Print the buffer contents
        } else
            cout << "Consumer error condition" << endl;    // Error (should not execute)

        pthread_mutex_unlock(&mutex_);  // Release the mutex_ lock
        sem_post(&empty_);              // Call post on the empty_ semaphore
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) { // Check if more or less arguments were specified for execution
        cout << "Expected 3 arguments, received: " << argc - 1 << endl;

        return 1; // Exit the program
    }

    // Declare variables and set arguments
    int sleep_time = atoi(argv[1]); // Time for threads to sleep before terminating
    int num_prods = atoi(argv[2]);  // Number of producer threads to create
    int num_cons = atoi(argv[3]);   // Number of consumer threads to create
    pthread_t producers[num_prods]; // Initialize producer thread array
    pthread_t consumers[num_cons];  // Initialize consumer thread array

    // Initialize the mutex_ lock
    if(pthread_mutex_init(&mutex_, NULL) != 0) {
        perror("pthread_mutex_init error");
        exit(1);
    }  

    // Initialize the full_ semaphore
    if(sem_init(&full_, 0, 0) != 0)
    {
        perror("sem_init full_ error");
        exit(1); 
    }

    // Initialize the empty_ semaphore
    if(sem_init(&empty_, 0, 5) != 0)
    {
        perror("sem_init empty_ error");
        exit(1); 
    }

    // Create producer thread(s).
    // Each thread has unique int ID to each producer thread, starting from 1 to number of threads
    for(int i = 0; i < num_prods; i++)
        pthread_create(&producers[i], NULL, producer, &i); // Each producer thread will run the producer function
    
    //Create consumer thread(s)
    for(int i = 0; i < num_cons; i++) 
        pthread_create(&consumers[i], NULL, consumer, NULL); // Each consumer thread will run the consumer function

    sleep(sleep_time); // Main thread will sleep for specified amount of time

    return 0; // Exit the program
}
