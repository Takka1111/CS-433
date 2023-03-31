/**
* Assignment 3: CPU Scheduler
 * @file scheduler_rr.h
 * @author Tucker Shaw and Abraham Gomez
 * @brief This Scheduler class implements the RoundRobin (RR) scheduling algorithm.
 * @version 0.1
 */

#ifndef ASSIGN3_SCHEDULER_RR_H
#define ASSIGN3_SCHEDULER_RR_H

#include "scheduler.h"
#include <string>       // std::string
#include <queue>        // std::queue

// Define global variables and structures
const int SIZE = 100; // # of processes possible

/**
 * @brief Struct stores results for process turnaround and waiting times
 *        It also stores the burst time changes as the time quantum is executed
 */
struct ProcTime_R {
    string name = "";       // Name of the process
    unsigned int burst = 0; // Burst time of process
    int turnaround = 0;     // Turnaround time of process
    int waiting = 0;        // Waiting time of process
};

class SchedulerRR : public Scheduler {
private:
    queue<PCB> ready_queue;     // Ready queue for processes
    unsigned int tquantum;      // For storing time quantum
    ProcTime_R results[SIZE];   // Array of ProcTime structs for printing results of algorithm
    int count;                  // Counter for results array

public:
    /**
     * @brief Construct a new SchedulerRR object
     */
    SchedulerRR(int time_quantum = 10);

    /**
     * @brief Destroy the SchedulerRR object
     */
    ~SchedulerRR() override;

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

#endif //ASSIGN3_SCHEDULER_RR_H
