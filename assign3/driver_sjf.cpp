/**
 * Driver (main) program for SJF scheduling algorithm.
 * The input file is a text file containing the process information in the following format:
 * [name] [priority] [CPU burst]
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "scheduler_sjf.h"

using namespace std;

int main(int argc, char *argv[]) {
    cout << "CS 433 Programming assignment 3" << endl;
    cout << "Author: Tucker Shaw and Abraham Gomez" << endl;
    cout << "Date: 03/31/2023" << endl;
    cout << "Course: CS433 (Operating Systems)" << endl;
    cout << "Description : test SJF scheduling algorithm " << endl;
    cout << "=================================" << endl;

    // Make sure the user has provided the input file name
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        exit(1);
    }

    // Read the input file
    ifstream input_file(argv[1]);
    
    // Make sure the file is open
    if (!input_file.is_open()) {
        cerr << "Error: Unable to open file " << argv[1] << endl;
        exit(1);
    }
    
    string line;
    vector<PCB> process_list;
    int id = 0;
    
    while(getline(input_file, line)) {
        // Split the line into tokens
        istringstream ss(line);
        string name;

        // parse out the name
        getline(ss, name, ',');
        
        // parse out the priority
        string token;
        getline(ss, token, ',');
        unsigned int priority = stoi(token);
        
        // parse out the burst time
        getline(ss, token, ',');
        unsigned int burst_time = stoi(token);

        // Create a PCB object and add it to the process list
        PCB pcb(name, id, priority, burst_time);
        id++;
        pcb.print();
        process_list.push_back(pcb);
    }

    // Create a scheduler object
    SchedulerSJF scheduler;
    
    // Run the scheduler
    scheduler.init(process_list);
    scheduler.simulate();
    scheduler.print_results();
}