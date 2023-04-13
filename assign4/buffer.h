/**
* Assignment 4: Producer Consumer Problem
 * @file buffer.h
 * @author Abraham Gomez and Tucker Shaw 
 * @brief header file for the buffer class
 * @version 0.1
 */

#ifndef ASSIGN4_BUFFER_H
#define ASSIGN4_BUFFER_H

#include <queue>        // For std::queue
#include <pthread.h>    // For mutex locks and condition variables

typedef int buffer_item; // The data type of the buffer items

/**
 * @brief Struct stores the semaphores for the producer-consumer threads
 * Contains:
    * A mutex lock
    * A condition variable for the buffer being full
    * A condition variable for the buffer being empty
 */
struct Lock {
    pthread_mutex_t mutex;  // The mutex lock
    pthread_cond_t full;    // Condition for full buffer
    pthread_cond_t empty;   // Condition for empty buffer
};

/**
 * @brief The bounded buffer class. The number of items in the buffer cannot exceed the size of the buffer.
 * This class contains:
    * A queue object for the buffer
    * The maximum capacity of the buffer
    * The current number of items in the buffer
    * A lock struct to hold the mutex, empty, and full semaphores
 */
class Buffer {
private:
    std::queue<buffer_item> buffer; // Buffer queue
    int capacity;                   // Maximum capacity
    int count;                      // Current number of items in buffer
    Lock lock;                      // Semaphore struct
    bool noError;                   // Flag for error conditions in producer/consumer
public:
    /**
     * @brief Construct a new Buffer object
     * @param size the size of the buffer
     */
    Buffer(int size = 5);

    /**
     * @brief Destroy the Buffer object
     */
    ~Buffer() {}

    /**
     * @brief Insert an item into the buffer
     * @param item the item to insert
     * @return true if successful
     * @return false if not successful
     */
    bool insert_item(buffer_item item);

    /**
     * @brief Remove an item from the buffer
     * @param item the item to remove
     * @return true if successful
     * @return false if not successful
     */
    bool remove_item(buffer_item *item);

    /**
     * @brief Get the size of the buffer
     * @return the size of the buffer
     */
    int get_size() { return this->capacity; } 

    /**
     * @brief Get the number of items in the buffer
     * @return the number of items in the buffer
     */
    int get_count() { return this->count; }

    /**
     * @brief Chceck if the buffer is empty
     * @return true if the buffer is empty, else false
     */
    bool is_empty() { return (count == 0) ? true:false; }

    /**
     * @brief Check if the buffer is full
     * @return true if the buffer is full, else false
     */
    bool is_full() { return (count == capacity) ? true:false; }

    /**
     * @brief Print the buffer
     */
    void print_buffer();
};
#endif //ASSIGN4_BUFFER_H
