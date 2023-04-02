/**
* Assignment 3: CPU Scheduler
 * @file scheduler_fcfs.cpp
 * @author Tucker Shaw and Abraham Gomez
 * @brief This Scheduler class implements the FCSF scheduling algorithm.
 * @version 0.1
 */

#include "scheduler_fcfs.h"

/**
* @brief Construct a new SchedulerFCFS object
*/
SchedulerFCFS::SchedulerFCFS() {
    this->count = 0; // Set counter for results array
}

/**
 * @brief This function is called once before the simulation starts.
 *        It is used to initialize the scheduler.
 * @param process_list The list of processes in the simulation.
 */
void SchedulerFCFS::init(std::vector<PCB>& process_list) {
    // Iterate through proc list and add to results array and ready queue
    for(std::vector<PCB>::iterator i = process_list.begin(); i != process_list.end(); i++) {
        this->results[this->count++].name = i->name; // Add the proc into result array
        this->ready_queue.push(*i); //Push in proc
    }
}

/**
 * @brief This function is called once after the simulation ends.
 *        It is used to print out the results of the simulation.
 */
void SchedulerFCFS::print_results() {
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
void SchedulerFCFS::simulate() {
    int total_time = 0; // For calculating turnaround and waiting times

    // Simulate FCFS scheduler until no more processes left
    while(!this->ready_queue.empty()) {
        std::cout << "Running Process " << this->ready_queue.front().name 
            << " for " << this->ready_queue.front().burst_time << " time units" << std::endl; // Print running message
        
        this->results[this->ready_queue.front().id].waiting = total_time; // Waiting time is just total time

        total_time += this->ready_queue.front().burst_time; // Update total time with burst time of current proc

        this->results[this->ready_queue.front().id].turnaround = total_time; // Turnaround time is just current time

        this->ready_queue.pop(); // Remove the process from the ready queue
    }
}