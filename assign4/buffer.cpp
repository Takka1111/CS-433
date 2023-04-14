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
    this->noError = false;  // Initialize the error condition flag
}

/**
 * @brief Insert an item into the buffer
 * @param item the item to insert
 * @return true if successful
 * @return false if not successful
 */
bool Buffer::insert_item(buffer_item item)
{
    // Check if the buffer is full  
    if(!is_full()) {
        this->buffer.push(item); // Add the item to the buffer
        this->count++;           // Increment the number of items in the buffer
        this->noError = true;
    }
    else // Buffer is full
        this->noError = false; // This should not be set

    return this->noError; // Return the result flag
}

 /**
 * @brief Remove an item from the buffer
 * @param item the item to remove
 * @return true if successful
 * @return false if not successful
 */
bool Buffer::remove_item(buffer_item *item)
{
    // Check if buffer is empty
    if(!is_empty()) {
        *item = this->buffer.front();   // Capture the front item
        this->buffer.pop();             // Remove the item from the buffer
        this->count--;                  // Decrement the number of items in the buffer
        this->noError = true;
    }
    else // Buffer is empty
        this->noError = false; // This should not be set

    return this->noError; // Return the result flag
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