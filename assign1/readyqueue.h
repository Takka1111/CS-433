/*
 * Assignment 1: priority queue of processes
 * @file readyqueue.h
 * @author Tucker Shaw and Abraham Gomez
 * @brief ReadyQueue is a queue of PCB's that are in the READY state to be scheduled to run.
 * It should be a priority queue such that the process with the highest priority can be selected next.
 * @version 0.1
*/

#pragma once

#include "pcb.h"

/*
 * @brief A queue of PCB's that are in the READY state to be scheduled to run.
 * It should be a priority queue such that the process with the highest priority can be selected next.
*/
class ReadyQueue {
 private:
  PCB* maxHeap[200]; //Ready Queue is a static array of PCB ptrs
  unsigned int count; //Stores the current num of PCBs in queue

 public:
  /*
   * @brief Construct a new ReadyQueue object
   *
  */
  ReadyQueue();

  /*
   * @brief Destructor
  */
  ~ReadyQueue() {}

  // You may add additional member functions, but don't change the definitions of the following four member functions.

  /*
   * @brief Add a PCB representing a process into the ready queue.
   *
   * @param pcbPtr: the pointer to the PCB to be added
  */
  void addPCB(PCB* pcbPtr);

  /*
   * @brief Remove and return the PCB with the highest priority from the queue
   *
   * @return PCB*: the pointer to the PCB with the highest priority
  */
  PCB* removePCB();

  /*
   * @brief Returns the number of elements in the queue.
   *
   * @return int: the number of PCBs in the queue
  */
  int size();

  /*
   * @brief Display the PCBs in the queue.
  */
  void displayAll();

  /*
   * @brief Swaps two PCB ptrs from the queue
  */
  void exchange(PCB*& elem1, PCB*& elem2);


  /*
   * @brief Fixes any heap violations from a specified index going up the heap
  */
  void bubbleUp(unsigned int idx);


  /*
   * @brief Fixes any heap violations from a specified index going down the heap
  */
  void maxHeapify(unsigned int idx);

};
