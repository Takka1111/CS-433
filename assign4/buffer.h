/**
* Assignment 4: Producer Consumer Problem
 * @file buffer.h
 * @author Abraham Gomez and Tucker Shaw 
 * @brief header file for the buffer class
 * @version 0.1
 */

#ifndef BUFFER_H
#define BUFFER_H

#include <queue> // For std::queue

typedef int buffer_item; // The data type of the buffer items

/**
 * @brief The bounded buffer class. The number of items in the buffer cannot exceed the size of the buffer.
 * This class contains:
    * A queue object for the buffer
    * The maximum capacity of the buffer
    * A flag for error conditions
 */
class Buffer {
private:
    std::queue<buffer_item> buffer; // Buffer queue
    long unsigned int capacity;     // Maximum capacity of buffer
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
    int get_count() { return this->buffer.size(); }

    /**
     * @brief Chceck if the buffer is empty
     * @return true if the buffer is empty, else false
     */
    bool is_empty() { return (this->buffer.size() == 0) ? true:false; }

    /**
     * @brief Check if the buffer is full
     * @return true if the buffer is full, else false
     */
    bool is_full() { return (this->buffer.size() == capacity) ? true:false; }

    /**
     * @brief Print the buffer
     */
    void print_buffer();
};

#endif //ASSIGN4_BUFFER_H
