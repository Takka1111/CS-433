#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <vector>

#include "fifo_replacement.h"
#include "lru_replacement.h"
#include "lifo_replacement.h"

// Check if an integer is power of 2
bool isPowerOfTwo(unsigned int x) {
    return x && (!(x & (x - 1)));   // First x in the expression is for the case when x is 0
}

int main(int argc, char *argv[]) {
    // Print basic information about the program
    std::cout << "=================================================================" << std::endl;
    std::cout << "CS 433 Programming assignment 5" << std::endl;
    std::cout << "Author: Abraham Gomez and Tucker Shaw" << std::endl;
    std::cout << "Date: 05/12/2023" << std::endl;                                       // CHANGE DATE IF NECESSARY
    std::cout << "Course: CS433 (Operating Systems)" << std::endl;
    std::cout << "Description : Program to simulate different page replacement algorithms" << std::endl;
    std::cout << "=================================================================\n" << std::endl;

    if (argc < 3) {
        // User does not enter enough parameters
        std::cout << "You have entered too few parameters to run the program.  You must enter" << std::endl
                  << "two command-line arguments:" << std::endl
                  << " - page size (in bytes): between 256 and 8192, inclusive" << std::endl
                  << " - physical memory size (in megabytes): between 4 and 64, inclusive" << std::endl;
        exit(1);
    }

    // Page size and Physical memory size
    unsigned int page_size = atoi(argv[1]); // Their values should be read from command-line arguments, and always a power of 2

    // Page Size is not a power of 2, display message and exit the program
    if (!isPowerOfTwo(page_size)) {
        std::cout << "You have entered an invalid parameter for page size (bytes)" << std::endl
                  << "  (must be an power of 2 between 256 and 8192, inclusive)." << std::endl;
        return 1;
    }

    unsigned int phys_mem_size = atoi(argv[2]) << 20; // convert from MB to bytes
    
    // Physical memory size is not a power of 2, display message and exit the program
    if (!isPowerOfTwo(phys_mem_size)) {
        std::cout << "You have entered an invalid parameter for physical memory size (MB)" << std::endl
                  << "  (must be an even integer between 4 and 64, inclusive)." << std::endl;
        return 1;
    }

    // Calculate number of pages and frames
    int logic_mem_bits = 27;                                    // 27-bit logical memory (128 MB logical memory assumed by the assignment)
    int phys_mem_bits = std::log2(phys_mem_size);               // Num of bits for physical memory addresses, calculated from physical memory size, e.g. 24 bits for 16 MB memory
    int page_offset_bits = std::log2(page_size);                // Num of bits for page offset, calculated from page size, e.g. 12 bits for 4096 byte page
    int num_pages = 1 << (logic_mem_bits - page_offset_bits);   // Number of pages in logical memory = 2^(logic_mem_bits - page_bit)
    int num_frames = 1 << (phys_mem_bits - page_offset_bits);   // Number of free frames in physical memory = 2^(phys_mem_bits - page_offset_bits)

    // Display page size, physical mem size, num pages, and num physical frames being used
    std::cout << "Page size = " << page_size << " bytes" << std::endl;
    std::cout << "Physical Memory size = " << phys_mem_size << " bytes" << std::endl;
    std::cout << "Number of pages = " << num_pages << std::endl;
    std::cout << "Number of physical frames = " << num_frames << std::endl;

    // Test 1: Read and simulate the small list of logical addresses from the input file "small_refs.txt"
    std::cout << "\n================================Test 1==================================================\n";
    
    std::ifstream in;

    in.open("small_refs.txt"); // Open the small reference file
    
    // Small reference file cannot be opened, display message and exit program
    if (!in.is_open()) {
        std::cerr << "Cannot open small_refs.txt to read. Please check your path." << std::endl;
        return 1;
    }

    int val;

    std::vector<int> small_refs; // Create a vector to store the logical addresses

    // Push in all logical addresses to the newly created vector
    while (in >> val) {
        small_refs.push_back(val);
    }

    // Create a virtual memory simulation using FIFO replacement algorithm
    FIFOReplacement vm(num_pages, num_frames); // Construct a FIFOReplacement page table object

    // Iterate through the logical addresses and simulate FIFO Replacement
    for (std::vector<int>::const_iterator it = small_refs.begin(); it != small_refs.end(); ++it) {
        int page_num = (*it) >> page_offset_bits; // Set the page number by taking the logical address and offsetting it

        bool isPageFault = vm.access_page(page_num, 0); // Check if there is a page fault and set a flag
        
        PageEntry pg = vm.getPageEntry(page_num); // Get the page entry from the page table

        // Display current logical address, page number, frame number, and whether there is a page fault
        std::cout << "Logical address: " << *it << ", \tpage number: " << page_num;
        std::cout << ", \tframe number = " << pg.frame_num << ", \tis page fault? " << isPageFault << std::endl;
    }

    in.close(); // Close the small reference file
    
    vm.print_statistics(); // Display the current statistics

    // Test 2: Read and simulate the large list of logical addresses from the input file "large_refs.txt"
    std::cout << "\n================================Test 2==================================================\n";

    std::cout << "Total number of references: " << 2000000 << std::endl;

    in.open("large_refs.txt"); // Open the small reference file
    
    // Small reference file cannot be opened, display message and exit program
    if (!in.is_open()) {
        std::cerr << "Cannot open large_refs.txt to read. Please check your path." << std::endl;
        return 1;
    }

    std::vector<int> large_refs; // Create a vector to store the logical addresses

    // Push in all logical addresses to the newly created vector
    while (in >> val) {
        large_refs.push_back(val);
    }

    in.close(); // Close the large reference file

    std::cout << "****************Simulate FIFO replacement****************************" << std::endl;

    // Create a virtual memory simulation using FIFO replacement algorithm
    FIFOReplacement fifoVM(num_pages, num_frames); // Construct a FIFOReplacement page table object

    auto startTime = std::chrono::steady_clock::now(); // Get the starting time

    // Iterate through the logical addresses and simulate FIFO Replacement
    for (std::vector<int>::const_iterator it = large_refs.begin(); it != large_refs.end(); ++it) {
        int page_num = (*it) >> page_offset_bits; // Set the page number by taking the logical address and offsetting it

        fifoVM.access_page(page_num, 0); // Check if there is a page fault
        
        fifoVM.getPageEntry(page_num); // Get the page entry from the page table
    }

    auto endTime = std::chrono::steady_clock::now(); // Get the ending time
    auto duration = std::chrono::duration<double>(endTime - startTime); // Get the duration of the time
    
    fifoVM.print_statistics(); // Display the current statistics

    std::cout << "Elapsed time = " << duration.count() << " seconds" << std::endl; // Print run-time

    std::cout << "****************Simulate LIFO replacement****************************" << std::endl;
    
    // Create a virtual memory simulation using FIFO replacement algorithm
    LIFOReplacement lifoVM(num_pages, num_frames); // Construct a FIFOReplacement page table object

    startTime = std::chrono::steady_clock::now(); // Get the starting time

    // Iterate through the logical addresses and simulate FIFO Replacement
    for (std::vector<int>::const_iterator it = large_refs.begin(); it != large_refs.end(); ++it) {
        int page_num = (*it) >> page_offset_bits; // Set the page number by taking the logical address and offsetting it

        lifoVM.access_page(page_num, 0); // Check if there is a page fault
        
        lifoVM.getPageEntry(page_num); // Get the page entry from the page table
    }

    endTime = std::chrono::steady_clock::now(); // Get the ending time
    duration = std::chrono::duration<double>(endTime - startTime); // Get the duration of the time
    
    lifoVM.print_statistics(); // Display the current statistics

    std::cout << "Elapsed time = " << duration.count() << " seconds" << std::endl; // Print run-time

    std::cout << "****************Simulate LRU replacement****************************" << std::endl;
    
    // Create a virtual memory simulation using FIFO replacement algorithm
    LRUReplacement lruVM(num_pages, num_frames); // Construct a FIFOReplacement page table object

    startTime = std::chrono::steady_clock::now(); // Get the starting time

    // Iterate through the logical addresses and simulate FIFO Replacement
    for (std::vector<int>::const_iterator it = large_refs.begin(); it != large_refs.end(); ++it) {
        int page_num = (*it) >> page_offset_bits; // Set the page number by taking the logical address and offsetting it

        lruVM.access_page(page_num, 0); // Check if there is a page fault
        
        lruVM.getPageEntry(page_num); // Get the page entry from the page table
    }

    endTime = std::chrono::steady_clock::now(); // Get the ending time
    duration = std::chrono::duration<double>(endTime - startTime); // Get the duration of the time
    
    lruVM.print_statistics(); // Display the current statistics

    std::cout << "Elapsed time = " << duration.count() << " seconds" << std::endl; // Print run-time

    return 0;
}
