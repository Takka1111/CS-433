/**
* Assignment 4: Producer Consumer Problem
 * @file buffer.cpp
 * @author Abraham Gomez and Tucker Shaw
 * @brief Implementation file for the buffer class
 * @version 0.1
 */

#include "buffer.h"
#include <iostream>

/**
 * @brief Construct a new Buffer object
 * @param size the size of the buffer
 */
Buffer::Buffer(int size)
{
    this->capacity = size;  // Set the capacity of the buffer
    this->count = 0;        // Initialize the number of items in the buffer
    
    // Initialize the mutex lock
    if(pthread_mutex_init(&lock.mutex, NULL) != 0) {
        perror("pthread_mutex_init");
        exit(1);
    }  

    // Initialize the full condition variable
    if(pthread_cond_init(&lock.full, NULL) != 0) {
        perror("pthread_cond_init");
        exit(1);    
    }

    // Initialize the empty condition variable
    if(pthread_cond_init(&lock.empty, NULL) != 0) {
        perror("pthread_cond_init");
        exit(1);    
    } 
}

/**
 * @brief Insert an item into the buffer
 * @param item the item to insert
 * @return true if successful
 * @return false if not successful
 */
bool Buffer::insert_item(buffer_item item)
{
    pthread_mutex_lock(&lock.mutex); // Acquire the mutex lock

    while(is_full())  // Check if buffer is full
        pthread_cond_wait(&lock.empty, &lock.mutex); // Producer sleeps and waits for signal
    
    this->buffer.push(item); // Add the item to the buffer
    this->count++;           // Increment the number of items in the buffer

    // Check if successful production and set appropriate flag
    if(this->count > 0 && this->count <= this->capacity)
        this->noError = true; 
    else
        this->noError = false;

    pthread_cond_signal(&lock.full);    // Signal the full condition variable for consumer threads
    pthread_mutex_unlock(&lock.mutex);  // Release the mutex lock

    return this->noError;   // Return the flag for error condtions
}

 /**
 * @brief Remove an item from the buffer
 * @param item the item to remove
 * @return true if successful
 * @return false if not successful
 */
bool Buffer::remove_item(buffer_item *item)
{
    pthread_mutex_lock(&lock.mutex); // Acquire the mutex lock

    while(is_empty())  // Check if queue is empty
        pthread_cond_wait(&lock.full, &lock.mutex); // Producer sleeps and waits for signal
    
    *item = this->buffer.front();   // Capture the front item
    this->buffer.pop();             // Remove the item from the buffer
    this->count--;                  // Decrement the number of items in the buffer

    // Check if successful consumption and set appropriate flag
    if(this->count >= 0 && this->count < this->capacity)
        this->noError = true; 
    else
        this->noError = false;

    pthread_cond_signal(&lock.empty);   // Signal the empty condition variable for producer threads
    pthread_mutex_unlock(&lock.mutex);  // Release the mutex lock

    return this->noError;   // Return the flag for error conditions
}

/**
 * @brief Print the buffer
 */
void Buffer::print_buffer()
{
    std::cout << "Buffer: [";

    // Check if buffer is empty
    if(!is_empty()) {

        // Create a copy of the buffer and iterate through items
        for(std::queue<buffer_item> items = this->buffer; !items.empty(); items.pop()) {
            std::cout << items.front();

            if(items.size() != 1) // Check if only one item in buffer or last item
                std::cout << ", ";
        }
    }

    std::cout << "]" << std::endl;
}