/**
* Assignment 3: CPU Scheduler
 * @file scheduler_rr.h
 * @author Tucker Shaw and Abraham Gomez
 * @brief This Scheduler class implements the RoundRobin (RR) scheduling algorithm.
 * @version 0.1
 */

#include "scheduler_rr.h"

/**
 * @brief Construct a new SchedulerRR object
 */
SchedulerRR::SchedulerRR(int time_quantum) {
    this->ready_queue = new std::queue<PCB>;    // Create a new ready queue
    this->tquantum = time_quantum;              // Store time quantum for simulation
    this->count = 0;                            // Set counter for results array
}

/**
 * @brief Destroy the SchedulerRR object
 */
SchedulerRR::~SchedulerRR() {
    delete this->ready_queue; // Destroy the ready queue
}

/**
 * @brief This function is called once before the simulation starts.
 *        It is used to initialize the scheduler.
 * @param process_list The list of processes in the simulation.
 */
void SchedulerRR::init(std::vector<PCB>& process_list) {
    // Iterate through unsorted process_list and add into results array
    for(std::vector<PCB>::iterator i = process_list.begin(); i != process_list.end(); i++) {
        this->ready_queue->push(*i);                            // Push in proc to ready_queue
        this->results[this->count++].name = i->name;            // Add the proc name into result array
        this->results[this->count - 1].burst = i->burst_time;   // Add the proc burst time into result array
    }
}

/**
 * @brief This function is called once after the simulation ends.
 *        It is used to print out the results of the simulation.
 */
void SchedulerRR::print_results() {
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
void SchedulerRR::simulate() {
    int total_time = 0; // For calculating turnaround and waiting times

    // Simulate RR scheduler until no more processes left
    while(!this->ready_queue->empty()) {
        std::cout << "Running Process " << this->ready_queue->front().name; //Print proc name

        // Check if time_quantum is still needed
        if(this->tquantum < this->results[this->ready_queue->front().id].burst) {
            std::cout << " for " << this->tquantum << " time units" << std::endl; // Print running message
            
            total_time += this->tquantum; // Update total time
            
            this->results[this->ready_queue->front().id].burst -= this->tquantum; // Update proc burst time

            this->ready_queue->push(this->ready_queue->front()); // Add proc to the back of the queue
        }
        else { // Process will finish
            std::cout << " for " << this->results[this->ready_queue->front().id].burst << " time units" << std::endl; // Print running message

            total_time += this->results[this->ready_queue->front().id].burst; // Update total time

            this->results[this->ready_queue->front().id].waiting = total_time - this->ready_queue->front().burst_time; // Waiting time is turnaround - burst time

            this->results[this->ready_queue->front().id].turnaround = total_time; // Turnaround is just total time
        }

        this->ready_queue->pop(); // Remove the process from the ready queue
    }
}