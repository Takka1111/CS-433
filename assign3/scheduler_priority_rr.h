/**
* Assignment 3: CPU Scheduler
 * @file scheduler_priority_rr.h
 * @author Tucker Shaw and Abraham Gomez
 * @brief This Scheduler class implements the Priority RR scheduling algorithm.
 * @version 0.1
 */

#ifndef ASSIGN3_SCHEDULER_PRIORITY_RR_H
#define ASSIGN3_SCHEDULER_PRIORITY_RR_H

#include "scheduler.h"
#include <string>       // std::string
#include <queue>        // std::queue

// Define global variables and structures
const int SIZE = 100; // # of processes possible

/**
 * @brief Struct stores results for process turnaround and waiting times
 *        It also stores the burst time changes as the time quantum is executed
 */
struct ProcTime_P_R {
    string name = "";           // Name of the process
    unsigned int burst = 0;     // Burst time of process
    unsigned int priority = 0;  // Priority of process (default 0 for unique priority)
    int turnaround = 0;         // Turnaround time of process
    int waiting = 0;            // Waiting time of process
};

class SchedulerPriorityRR : public Scheduler {
private:
    queue<PCB> ready_queue;     // Ready queue for processes
    unsigned int tquantum;      // For storing time quantum
    ProcTime_P_R results[SIZE]; // Array of ProcTime structs for printing results of algorithm
    int count;                  // Counter for results array

public:
    /**
     * @brief Construct a new SchedulerPriority object
     */
    SchedulerPriorityRR(int time_quantum = 10);

    /**
     * @brief Destroy the SchedulerPriority object
     */
    ~SchedulerPriorityRR() override {} // Declaring empty destructor (nothing in memory to destroy)

    /**
     * @brief This function is called once before the simulation starts.
     *        It is used to initialize the scheduler.
     * @param process_list The list of processes in the simulation.
     */
    void init(std::vector<PCB>& process_list) override;

    /**
     * @brief This function is called once after the simulation ends.
     *        It is used to print out the results of the simulation.
     */
    void print_results() override;

    /**
     * @brief This function simulates the scheduling of processes in the ready queue.
     *        It stops when all processes are finished.
     */
    void simulate() override;

};

#endif //ASSIGN3_SCHEDULER_PRIORITY_RR_H
