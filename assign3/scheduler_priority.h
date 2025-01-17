/**
* Assignment 3: CPU Scheduler
 * @file scheduler_priority.h
 * @author Tucker Shaw and Abraham Gomez
 * @brief This Scheduler class implements the Priority scheduling algorithm.
 * @version 0.1
 */

#ifndef ASSIGN3_SCHEDULER_PRIORITY_H
#define ASSIGN3_SCHEDULER_PRIORITY_H

#include "scheduler.h"
#include <string>       // std::string
#include <queue>        // std::queue

// Define global variables and structures
const int SIZE = 100; // # of processes possible

/**
 * @brief Struct stores results for process turnaround and waiting times
 * 
 */
struct ProcTime_P {
    string name = "";   // Name of the process
    int turnaround = 0; // Turnaround time of process
    int waiting = 0;    // Waiting time of process
};

class SchedulerPriority : public Scheduler {
private:
    queue<PCB> ready_queue;     // Ready queue for processes
    ProcTime_P results[SIZE];   // Array of ProcTime structs for printing results of algorithm
    int count;                  // Counter for results array

public:
    /**
     * @brief Construct a new SchedulerPriority object
     */
    SchedulerPriority();

    /**
     * @brief Destroy the SchedulerPriority object
     */
    ~SchedulerPriority() override {} // Declaring empty destructor (nothing in memory to destroy)

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

#endif //ASSIGN3_SCHEDULER_PRIORITY_H
