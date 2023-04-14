/**
* Assignment 4: Producer Consumer Problem
 * @file main.cpp
 * @author Abraham Gomez and Tucker Shaw
 * @brief The main program for the producer consumer problem.
 * @version 0.1
 */

#include <iostream>
#include <unistd.h>
#include "buffer.h"

using namespace std;

Buffer buffer; // Global buffer object

/**
 * @brief Producer function
 * @param param The ID for the buffer item
 */
void *producer(void *param) {
    buffer_item item = *((int *) param); // Produce an item with unique ID

    // Thread will continue to loop until main exits
    while (true) {
        usleep(rand()%1000000); // Sleep for a random period of time

        if (buffer.insert_item(item)) {
            cout << "Producer " << item << ": Inserted item " << item << endl;

            buffer.print_buffer(); // Print the buffer contents
        } else {
            cout << "Producer error condition"  << endl;    // Error (should not execute)
        }
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

        // Consume an item and print result
        if (buffer.remove_item(&item)) {
            cout << "Consumer Removed item " << item << endl;

            buffer.print_buffer(); // Print the buffer contents
        } else {
            cout << "Consumer error condition" << endl;    // Error (should not execute)
        }
    }
}

int main(int argc, char *argv[]) {
    // Declare variables and set arguments
    int sleep_time = atoi(argv[1]); // Time for threads to sleep before terminating
    int num_prods = atoi(argv[2]);  // Number of producer threads to create
    int num_cons = atoi(argv[3]);   // Number of consumer threads to create
    pthread_t producers[num_prods]; // Initialize producer thread array
    pthread_t consumers[num_cons];  // Initialize consumer thread array
    //int id;                         // Container for producer's ID

    // Check if more or less arguments were specified for execution
    if (argc != 4) {
        cout << "Expected 3 arguments, received: " << argc - 1 << endl;

        return 1; // Exit the program
    }

    // Create producer thread(s).
    // Each thread has unique int ID to each producer thread, starting from 1 to number of threads
    for(int i = 0; i < num_prods; i++)
        pthread_create(&producers[i], NULL, &producer, (void *) &(i)); // Each producer thread will run the producer function
    
    //Create consumer thread(s)
    for(int i = 0; i < num_cons; i++) 
        pthread_create(&consumers[i], NULL, &consumer, NULL); // Each consumer thread will run the consumer function

    sleep(sleep_time); // Main thread sleep for specified amount of time

    return 0; // Exit the program
}
