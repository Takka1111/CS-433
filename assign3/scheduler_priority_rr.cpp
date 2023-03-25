/**
* Assignment 3: CPU Scheduler
 * @file scheduler_priority_rr.cpp
 * @author Tucker Shaw and Abraham Gomez
 * @brief This Scheduler class implements the Priority RR scheduling algorithm.
 * @version 0.1
 */

#include "scheduler_priority_rr.h"
#include <algorithm>                // std::sort

bool compareProc(PCB proc1, PCB proc2) { return proc1.priority > proc2.priority; } // For comparing processes to sort process_list

/**
 * @brief Construct a new SchedulerPriority object
 */
SchedulerPriorityRR::SchedulerPriorityRR(int time_quantum) {
    this->ready_queue = new std::queue<PCB>;    // Create a new ready queue
    this->tquantum = time_quantum;              // Store time quantum for simulation
    this->count = 0;                            // Set counter for results array
}

/**
 * @brief Destroy the SchedulerPriority object
 */
SchedulerPriorityRR::~SchedulerPriorityRR() {
    delete this->ready_queue; // Destroy the ready queue
}

/**
 * @brief This function is called once before the simulation starts.
 *        It is used to initialize the scheduler.
 * @param process_list The list of processes in the simulation.
 */
void SchedulerPriorityRR::init(std::vector<PCB>& process_list) {
    // Iterate through unsorted process_list and add into results array
    for(std::vector<PCB>::iterator i = process_list.begin(); i != process_list.end(); i++) {
        this->results[this->count++].name = i->name;            // Add the proc name into result array
        this->results[this->count - 1].burst = i->burst_time;   // Add the proc burst time into result array
    }

    std::sort(process_list.begin(), process_list.end(), compareProc); // Sort the list by highest priority

    // Iterate through proc list and fill ready queue
    for(std::vector<PCB>::iterator i = process_list.begin(); i != process_list.end(); i++) {
        this->ready_queue->push(*i); //Push in proc

        // Check if current process has same priority as next or previous
        if(i->priority == (i + 1)->priority || (i != process_list.begin() && i->priority == (i - 1)->priority))
            this->results[this->ready_queue->back().id].priority = this->ready_queue->back().priority; // Update priority in result array
    }
}

/**
 * @brief This function is called once after the simulation ends.
 *        It is used to print out the results of the simulation.
 */
void SchedulerPriorityRR::print_results() {
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
void SchedulerPriorityRR::simulate() {
    int total_time = 0; // For calculating turnaround and waiting times
    queue<PCB> temp;    // For duplicate priorities

    // Simulate Priority_RR scheduler until no more processes left
    while(!this->ready_queue->empty()) {

        // Check for duplicate priorities and place in temp queue
        while(this->results[ready_queue->front().id].priority != 0) {
            temp.push(this->ready_queue->front());  // Add to temp
            this->ready_queue->pop();               // Remove from ready_queue
        }

        // Check if no duplicate priorities were found
        if(temp.empty()) {
            std::cout << "Running Process " << this->ready_queue->front().name << " for " 
                      << this->ready_queue->front().burst_time << " time units" << std::endl; // Print running message
            
            this->results[this->ready_queue->front().id].waiting = total_time; // Get waiting time for current proc

            total_time += this->ready_queue->front().burst_time; // Update total time

            this->results[this->ready_queue->front().id].turnaround = total_time; // Turnaround is just total time

            this->ready_queue->pop(); // Remove the process from the ready queue
        }
        else { // Duplicate priorities

            // Iterate through temp queue until duplicates are finished
            while(!temp.empty()) {
                std::cout << "Running Process " << temp.front().name;

                // Check if time quantum still needed and queue has more than one element
                if(this->tquantum < this->results[temp.front().id].burst && temp.size() != 1) {
                    std::cout << " for " << this->tquantum << " time units" << std::endl; // Print running message
                
                    total_time += this->tquantum; // Update total time

                    this->results[temp.front().id].burst -= this->tquantum; // Update proc burst time

                    temp.push(temp.front()); // Add proc to the back of the queue
                }
                else { // Process will finish
                    std::cout << " for " << this->results[temp.front().id].burst << " time units" << std::endl; // Print running message

                    total_time += this->results[temp.front().id].burst; // Update total time

                    this->results[temp.front().id].waiting = total_time - temp.front().burst_time; // Waiting time is turnaround - burst time

                    this->results[temp.front().id].turnaround = total_time; // Turnaround is just total time
                }

                temp.pop(); // Remove process from temp queue
            }
        }
    }
}