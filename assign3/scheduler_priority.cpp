/**
* Assignment 3: CPU Scheduler
 * @file scheduler_priority.cpp
 * @author Tucker Shaw and Abraham Gomez
 * @brief This Scheduler class implements the Priority scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code


#include "scheduler_priority.h"
#include <algorithm>


bool compareProc(PCB proc1, PCB proc2){return proc1.priority > proc2.priority;}

/**
 * @brief Construct a new SchedulerPriority Object
*/
SchedulerPriority::SchedulerPriority(){
    this->ready_queue = new std::queue<PCB>;
    this->count = 0;
}

/**
 * @brief Destroy the SchedulerPriority Object
*/
SchedulerPriority::~SchedulerPriority(){
    delete this->ready_queue;
}

/**
 * @brief This function is called before the simulation
 * @param process_list The list of processes in the sim
*/
void SchedulerPriority::init(std::vector<PCB>& process_list){

    for(std::vector<PCB>::iterator i = process_list.begin(); i != process_list.end(); i++){
        this->results[this->count++].name = i->name;
	this->results[this->count - 1].burst = i->burst_time;
    }
    std::sort(process_list.begin(), process_list.end(), compareProc);
    for(std::vector<PCB>::iterator i = process_list.begin(); i != process_list.end(); i++){
	this->ready_queue->push(*i);
    }
}

/**
 * @brief Function is called after simulation ends and prints out the results of simulation
*/
void SchedulerPriority::print_results() {
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
 * @brief Function simulates the scheduling of the processes in the ready queue
*/
void SchedulerPriority::simulate() {
    int total = 0;

    while(!this->ready_queue->empty()){
        std::cout << "Running Process " << this->ready_queue->front().name << " for " << this->ready_queue->front().burst_time << " time units " <<  std::endl;
        
        this->results[this->ready_queue->front().id].waiting = total;
        total += this->ready_queue->front().burst_time;

        this->results[this->ready_queue->front().id].turnaround = total;

        this->ready_queue->pop();
    }
}