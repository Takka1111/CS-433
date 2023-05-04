/**
* Assignment 5: Page replacement algorithms
 * @file pagetable.h
 * @author Abraham Gomez and Tucker Shaw
 * @brief This class represents a traditional pagetable data structure.
 * @version 0.1
 */

#pragma once

#include <vector>       // for std::vector
using namespace std;

/**
 * @brief A page table entry. This class is used to represent a page table entry.
 * @details Each page has one entry in the page table. 
 * This class contains the following fields:
    * The physical frame number
    * The valid bit
    * The dirty bit (not used)
 */
class PageEntry
{
public:
	int frame_num;      // Physical frame number for a given page
	bool valid = false; // Valid bit represents whether a page is in the physical memory
    bool dirty = false; // Dirty bit represents whether a page is changed (not used in this assignment)
};

/**
 * @brief A page table is like an array of page entries.
 * The size of the page table should equal to the number of pages in logical memory
 * This class contains:
    * A vector to act as the array of page entries in the page table
 */
class PageTable
{
private:
    vector<PageEntry> pages;    // A page table is like an array of page entries
public:
    /**
     * @brief Construct a PageTable object
     * @param num_pages
     */
    PageTable(int num_pages);

    /**
     * @brief Deconstruct a PageTable object
     */
    ~PageTable() {};

    /**
     * @brief Access a page in the page table.
     * @param i
     * @return A reference to a page entry
     */
    PageEntry& operator [] (int i); // Operator overloader for []
};