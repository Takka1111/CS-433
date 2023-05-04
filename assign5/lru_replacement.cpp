/**
* Assignment 5: Page replacement algorithms
 * @file lru_replacement.cpp
 * @author Abraham Gomez and Tucker Shaw
 * @brief A class implementing the LRU page replacement algorithms
 * @version 0.1
 */

#include "lru_replacement.h"

/**
 * @brief Accesss a page already in physical memory
 * It may be overridden in a subclass 
 * @param page_num The logical page number.
 */
void LRUReplacement::touch_page(int page_num)
{
    auto i = this->frame_table.find(page_num);                  // Get the iterator from the map
    this->references.erase(i->second);                          // Erase the page number from its current spot
    this->references.push_front(page_num);                      // Push the new page number into the list at the front
    this->frame_table[page_num] = this->references.begin();     // Get the iterator and set it into the map
}

/**
 * @brief Access an invalid page, but free frames are available.
 * Assign the page to an available  frame, not replacement needed
 * @param page_num The logical page number.
 */
void LRUReplacement::load_page(int page_num) 
{
    this->references.push_front(page_num);                  // Push the new page number into the list
    this->frame_table[page_num] = this->references.begin(); // Add the list's iterator to the map
    this->page_table[page_num].valid = 1;                   // Set the new page number's valid bit to true
}

/**
 * @brief Access an invalid page, and there is no free frame.
 * Replace the page with the page that has been in memory the longest.
 * @param page_num The logical page number.
 * @return Selected victim page #
 */
int LRUReplacement::replace_page(int page_num) 
{
    int old_page_num = this->references.back();             // Get the old page number
    this->page_table[old_page_num].valid = 0;               // Set the old page number's valid bit to false

    this->references.pop_back();                            // Pop the old page number from the list
    this->frame_table.erase(old_page_num);                  // Erase that iterator from the map
    this->references.push_front(page_num);                  // Push the new page number into the list
    this->frame_table[page_num] = this->references.begin(); // Add the iterator to the map

    this->page_table[page_num].valid = 1;                   // Set the new page number's valid bit to true

    return 0;
}