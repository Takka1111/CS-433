/**
* Assignment 3: CPU Scheduler
 * @file scheduler_sjf.cpp
 * @author Tucker Shaw and Abraham Gomez
 * @brief This Scheduler class implements the SJF scheduling algorithm.
 * @version 0.1
 */

#include "scheduler_sjf.h"
#include <algorithm>        // std::sort

bool compareProc(PCB proc1, PCB proc2) { return proc1.burst_time < proc2.burst_time; } // For comparing processes to sort process_list

/**
* @brief Construct a new SchedulerSJF object
*/
SchedulerSJF::SchedulerSJF() {
    this->count = 0; // Set counter for results array
}

/**
 * @brief This function is called once before the simulation starts.
 *        It is used to initialize the scheduler.
 * @param process_list The list of processes in the simulation.
 */
void SchedulerSJF::init(std::vector<PCB>& process_list) {
    // Iterate through unsorted process_list and add into results array
    for(std::vector<PCB>::iterator i = process_list.begin(); i != process_list.end(); i++)
        this->results[this->count++].name = i->name; // Add the proc into result array

    std::sort(process_list.begin(), process_list.end(), compareProc); // Sort the list by shortest burst times

    // Iterate through proc list and fill ready queue
    for(std::vector<PCB>::iterator i = process_list.begin(); i != process_list.end(); i++)
        this->ready_queue.push(*i); //Push in proc
}

/**
 * @brief This function is called once after the simulation ends.
 *        It is used to print out the results of the simulation.
 */
void SchedulerSJF::print_results() {
    double avg_turnaround = 0, avg_waiting = 0; // For calculating averages
    
    // Loop through result array and print results
    for(int i = 0; i < this->count; i++) {
        std::cout << this->results[i].name << " turn-around time = " << this->results[i].turnaround
                  << ", waiting time = " << this->results[i].waiting << std::endl; // Print results
        
        //Store turnaround and waiting times
        avg_turnaround += this->results[i].turnaround;
        avg_waiting += this->results[i].waiting;
    }

    // Print average turnaround and waiting times
    std::cout << "Average turn-around time = " << avg_turnaround / this->count
              << ", Average waiting time = " << avg_waiting / this->count << std::endl;
}

/**
 * @brief This function simulates the scheduling of processes in the ready queue.
 *        It stops when all processes are finished.
 */
void SchedulerSJF::simulate() {
    int total_time = 0; // For calculating turnaround and waiting times

    // Simulate SJF scheduler until no more processes left
    while(!this->ready_queue.empty()) {
        std::cout << "Running Process " << this->ready_queue.front().name 
            << " for " << this->ready_queue.front().burst_time << " time units" << std::endl; // Print running message
        
        this->results[this->ready_queue.front().id].waiting = total_time; // Waiting time is just total time

        total_time += this->ready_queue.front().burst_time; // Update total time with burst time of current proc

        this->results[this->ready_queue.front().id].turnaround = total_time; // Turnaround time is just current time

        this->ready_queue.pop(); // Remove the process from the ready queue
    }
}