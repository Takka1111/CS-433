/**
* Assignment 5: Page replacement algorithms
 * @file replacement.h
 * @author Abraham Gomez and Tucker Shaw
 * @brief A base class for different page replacement algorithms.
 * @version 0.1
 */

#pragma once

#include "pagetable.h"

/**
 * @brief A struct to store statistics gathered when running algorithms
 * It contains 3 integer variables to store
    * Number of page replacements
    * Number of page faults
    * Number of references
    * Last page access time for the LRU algorithm
    * The total amount of time taken by the algorithm
 */
struct Statistics {
    int num_page_replacements = 0;  // Stores number of page replacements
    int num_page_faults = 0;        // Stores number of page faults
    int num_references = 0;         // Stores number of references
};

/**
 * @brief A base class to simulate page replacement algorithms.
 * A specific page replacement algorithm, e.g. FIFO or LRU, should be subclass of this.
 * This class contains:
    * A page table object
    * A Statistics struct to store results of the algorithms
    * A counter for the number of free frames left in the page table
 */
class Replacement
{
protected: // Subclasses can access these members
    PageTable page_table;   // Member variable for the page table
	Statistics stats;       // A Statistics struct for storing the results of the algorithms
    int free_frames;        // Stores the current number of free frames in the page table
	
public:
	/**
	 * @brief 
	 * @param num_pages Total number of logical pages for the simulation.
	 * @param num_frames Total number of available free frames.
	 */
	Replacement(int num_pages, int num_frames);

    /**
     * @brief Destructor
     */
    virtual ~Replacement() {};

    /**
	 * @brief Simulate a single page access.
     * @details If the page is valid, it calls the touch_page function. 
     *          If the page is not valid but free frames are available, it calls the load_page function.
     *          If the page is not valid and there is no free frame, it calls the replace_page function.
     * @param page_num The logical page number.
	 * @param is_write whether this access a memory write
	 * @return whether it's a page fault: true if it's a page fault
	 */
    virtual bool access_page(int page_num, bool is_write = false);

    /**
	 * @brief Accesss a page already in physical memory
	 * It may be overridden in a subclass 
	 * @param page_num The logical page number.
     */
    virtual void touch_page(int page_num) {}

    /**
     * @brief Access an invalid page, but free frames are available.
     * Assign the page to an available frame, no replacement needed
     * It may be overridden in a subclass 
     * @param page_num The logical page number.
     */
    virtual void load_page(int page_num) {}


    /**
	 * @brief Access an invalid page and no free frame is available.
     * Select a victim page to be replaced.
	 * It is a pure virtual function to be implemented in specific replacement subclasses.
     * @param page_num  The logical page number of the desired page.
	 * @return Selected victim page #
	 */
    virtual int replace_page(int page_num) = 0;

    /**
	 * @brief Get the ith entry of the page table
	 */
    PageEntry getPageEntry(int page_num) {
        return page_table[page_num]; // Call operator [] from pagetable.h
    }

    /**
	 * @brief Print the statistics of simulation
	 */
    void print_statistics() const;
};