/**
* Assignment 3: CPU Scheduler
 * @file scheduler_fcfs.cpp
 * @author Tucker Shaw and Abraham Gomez
 * @brief This Scheduler class implements the FCSF scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "scheduler_fcfs.h"

/**
* @brief Construct a new SchedulerFCFS object
*/
SchedulerFCFS::SchedulerFCFS() {
    this->ready_queue = new std::queue<PCB>;    // Create a new ready queue
    this->count = 0;                            // Set counter for results array
}

/**
 * @brief Destroy the SchedulerFCFS object
 */
SchedulerFCFS::~SchedulerFCFS() {
    delete this->ready_queue; // Destroy the ready queue
}

/**
 * @brief This function is called once before the simulation starts.
 *        It is used to initialize the scheduler.
 */
void SchedulerFCFS::init(std::vector<PCB>& process_list) {
    // Iterate through unsorted process_list and add into results array
    for(std::vector<PCB>::iterator i = process_list.begin(); i != process_list.end(); i++)
        this->results[this->count++].name = i->name; // Add the proc into result array

    // Iterate through proc list and fill ready queue
    for(std::vector<PCB>::iterator i = process_list.begin(); i != process_list.end(); i++)
        this->ready_queue->push(*i); //Push in proc
}

/**
 * @brief This function is called once after the simulation ends.
 *        It is used to print out the results of the simulation.
 */
void SchedulerFCFS::print_results() {
    double avg_turnaround, avg_waiting = 0; // For calculating averages
    
    // Loop through result array and print results
    for(int i = 0; i < count; i++) {
        std::cout << this->results[i].name << " turn-around time = " << this->results[i].turnaround
                  << ", waiting time = " << this->results[i].waiting << std::endl; // Print results
        
        //Store turnaround and waiting times
        avg_turnaround += this->results[i].turnaround;
        avg_waiting += this->results[i].waiting;
    }

    // Print average turnaround and waiting times
    std::cout << "Average turn-around time = " << avg_turnaround / 8
              << ", Average waiting time = " << avg_waiting / 8 << std::endl;
}

/**
 * @brief This function simulates the scheduling of processes in the ready queue.
 *        It stops when all processes are finished.
 */
void SchedulerFCFS::simulate() {
    int total_time = 0; // For calculating turnaround and waiting times

    // Simulate SJF scheduler until no more processes left
    while(!this->ready_queue->empty()) {
        std::cout << "Running Process " << this->ready_queue->front().name 
            << " for " << this->ready_queue->front().burst_time << " time units" << std::endl; // Print running message
        
        this->results[this->ready_queue->front().id].waiting = total_time; // Get waiting time for current proc

        total_time += this->ready_queue->front().burst_time; // Update total time

        this->results[this->ready_queue->front().id].turnaround = total_time; // Get turnaround time for current proc

        this->ready_queue->pop(); // Remove the process from the ready queue
    }
}