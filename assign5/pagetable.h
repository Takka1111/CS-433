/**
* Assignment 5: Page replacement algorithms
 * @file pagetable.h
 * @author Abraham Gomez and Tucker Shaw
 * @brief This class represents a traditional pagetable data structure.
 * @version 0.1
 */

#pragma once

#include <vector>       // for vector<PageEntry>
using namespace std;

// A page table entry
/**
 * @brief A page table entry. This class is used to represent a page table entry.
 * @details Each page has one entry in the page table. It contains the following fields:
 * - frame number
 * - valid bit
 */
class PageEntry
{
public:
	int frame_num;      // Physical frame number for a given page
	bool valid = false; // valid bit represents whether a page is in the physical memory
    bool dirty = false; // dirty bit represents whether a page is changed (not used in this assignment)
};


/**
 * @brief A page table is like an array of page entries.
 * The size of the page table should equal to the number of pages in logical memory
 */
class PageTable
{
private:
    // A page table is like an array of page entries.
    vector<PageEntry> pages;
public:
    // Constructor
    PageTable(int num_pages);
    // Destructor
    ~PageTable();

	// TODO: Add your implementation of the page table here

    /**
     * @brief Access a page in the page table.
     * @param i
     * @return
     */
    PageEntry& operator [] (int i) {
        return pages[i];
    }
};