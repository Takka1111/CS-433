/**
* Assignment 5: Page replacement algorithms
 * @file lru_replacement.h
 * @author Abraham Gomez and Tucker Shaw
 * @brief A class implementing the LRU page replacement algorithms
 * @version 0.1
 */

#pragma once

#include "replacement.h" 
#include <unordered_map>    // For std::unordered_map
#include <list>             // For std::list

/**
 * @brief A class to simulate the least recently used (LRU) page replacement algorithm.
 */
class LRUReplacement : public Replacement
{
    unordered_map<int, list<int>::iterator> frame_table;    // Create a map to store page numbers
    list<int> references;                                   // Create a list to track least recently used page number

public:
	/**
	 * @brief Constructor
	 * @param num_pages 
	 * @param num_frames 
	 */
	LRUReplacement(int num_pages, int num_frames) : Replacement(num_pages, num_frames) {}; 
	
    /**
    * @brief Destructor
    */
    virtual ~LRUReplacement() {};

    /**
     * @brief Accesss a page alreay in physical memory
     * It may be overridden in a subclass 
     * @param page_num The logical page number.
     */
    virtual void touch_page(int page_num);

    /**
     * @brief Access an invalid page, but free frames are available.
     * Assign the page to an available  frame, not replacement needed
     * @param page_num The logical page number.
     */
    virtual void load_page(int page_num);

    /**
     * @brief Access an invalid page, and there is no free frame.
     * Replace the page with the page that has been in memory the longest.
     * @param page_num The logical page number.
     * @return Selected victim page #
     */
    virtual int replace_page(int page_num);
};