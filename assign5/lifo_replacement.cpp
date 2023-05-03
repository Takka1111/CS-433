/**
* Assignment 5: Page replacement algorithms
 * @file lifo_replacement.cpp
 * @author Abraham Gomez and Tucker Shaw
 * @brief A class implementing the LIFO page replacement algorithms
 * @version 0.1
 */

#include "lifo_replacement.h"

/**
 * @brief Access an invalid page, but free frames are available.
 * Assign the page to an available  frame, not replacement needed
 * @param page_num The logical page number.
 */
void LIFOReplacement::load_page(int page_num) {
    this->frame_table.push(page_num);       // Push the page number into the frame table
    this->page_table[page_num].valid = 1;   // Set the current page number in the page table to have valid bit
}

/**
 * @brief Access an invalid page, and there is no free frame.
 * Replace the page with the page that has been in memory the longest.
 * @param page_num The logical page number.
 * @return Selected victim page #
 */
int LIFOReplacement::replace_page(int page_num) {
    int frame = this->frame_table.top();    // Get the current frame at front of table
    this->page_table[frame].valid = 0;      // Set that frame's page valid bit to 0
    this->frame_table.pop();                // Remove the front frame from the table

    this->frame_table.push(page_num);       // Push in the new page number into the frame table
    this->page_table[page_num].valid = 1;   // Set the new page number's valid bit to 1

    return 0;
}