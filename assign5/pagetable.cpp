/**
* Assignment 5: Page replacement algorithms
 * @file pagetable.cpp
 * @author Abraham Gomez and Tucker Shaw
 * @brief This class represents a traditional pagetable data structure.
 * @version 0.1
 */

#include "pagetable.h"

/**
 * @brief Construct a PageTable object
 * @param num_pages
 */
PageTable::PageTable(int num_pages) {
    this->pages.resize(num_pages); // Set the size of the PageTable vector to the number of pages in logical memory
}

/**
 * @brief Access a page in the page table.
 * @param i
 * @return A reference to a page entry
 */
PageEntry& PageTable::operator [] (int i) {
    return pages[i];
}