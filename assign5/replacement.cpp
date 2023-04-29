/**
* Assignment 5: Page replacement algorithms
 * @file replacement.cpp
 * @author Abraham Gomez and Tucker Shaw
 * @brief A base class for different page replacement algorithms.
 * @version 0.1
 */

#include <iostream>
#include "replacement.h"

// Constructor
Replacement::Replacement(int num_pages, int num_frames) : page_table(num_pages)
{
	this->free_frames = num_frames; // Set counter for number of free frames in memory
}

/**
 * @brief Simulate a single page access.
 * @details If the page is valid, it calls the touch_page function. 
 *          If the page is not valid but free frames are available, it calls the load_page function.
 *          If the page is not valid and there is no free frame, it calls the replace_page function.
 * @param page_num The logical page number.
 * @param is_write whether this access is a memory write
 * @return whether it's a page fault: true if it's a page fault
 */
bool Replacement::access_page(int page_num, bool is_write)
{
    bool isValid = this->page_table[page_num].valid;    // Check if the current page number is valid
    bool isFault = false;                               // Create a flag for a page fault

    if(isValid) // Page is valid
        this->touch_page(page_num); // Page already in physical memory, access it
    else { // Page is invalid
        if(this->free_frames != 0) // Free frames are available
            this->load_page(page_num); // Load invalid page into free frame
        else // No free frames available
            this->replace_page(page_num); // Replace a frame with current page, based on algorithms

        isFault = true;                 // Set flag for page fault
        this->stats.num_page_faults++;  // Update number of page faults
    }

    return isFault; // Return flag for page fault
}

/**
 * @brief Print the statistics of simulation
 */
void Replacement::print_statistics() const {
    // Print out the number of references, number of page faults and number of page replacements
    std::cout << "Number of references: \t\t" << this->stats.num_references << std::endl;
    std::cout << "Number of page faults: \t\t" << this->stats.num_page_faults << std::endl;
    std::cout << "Number of page replacements: \t" << this->stats.num_page_replacements << std::endl;
}