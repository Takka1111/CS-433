/*
 * Assignment 1: priority queue of processes
 * @file readyqueue.cpp
 * @author Tucker Shaw and Abraham Gomez
 * @brief ReadyQueue is a queue of PCB's that are in the READY state to be scheduled to run.
 * It should be a priority queue such that the process with the highest priority can be selected next.
 * @version 0.1
*/

#include <iostream>
#include "readyqueue.h"

using namespace std;

/*
 * @brief Constructor for the ReadyQueue class.
*/
ReadyQueue::ReadyQueue()  {
  count = 0;
  
  //Iterate through queue and set ptrs to NULL
  for(int i = 0; i < 200; i++) {
    maxHeap[i] = NULL;
  }
}

/*
 * @brief Add a PCB representing a process into the ready queue.
 *
 * @param pcbPtr: the pointer to the PCB to be added
*/
void ReadyQueue::addPCB(PCB *pcbPtr) {
  // When adding a PCB to the queue, you must change its state to READY.
  pcbPtr->setState(ProcState::READY);
    
  maxHeap[count++] = pcbPtr; //Add the PCB ptr to the queue
    
  bubbleUp(count - 1); //Fix any heap violations
}

/*
 * @brief Remove and return the PCB with the highest priority from the queue
 *
 * @return PCB*: the pointer to the PCB with the highest priority
*/
PCB* ReadyQueue::removePCB() {
  PCB* pcbPtr = maxHeap[0];
  
  exchange(maxHeap[0], maxHeap[count - 1]); //Swap PCBs from front and end of queue
  count--;
  
  maxHeapify(0); //Fix any heap violations
  
  pcbPtr->setState(ProcState::RUNNING); // When removing a PCB from the queue, you must change its state to RUNNING.
  return pcbPtr;
}

/*
 * @brief Returns the number of elements in the queue.
 *
 * @return int: the number of PCBs in the queue
*/
int ReadyQueue::size() {
  return count;
}

/*
 * @brief Display the PCBs in the queue.
*/
void ReadyQueue::displayAll() {
  cout << "Display Processes in ReadyQueue:" << endl;
  for(unsigned int i = 0; i < count; i++) {
    maxHeap[i]->display();
  }
}

/*
 * @brief Swap two PCB ptrs within the queue
*/
void ReadyQueue::exchange(PCB*& elem1, PCB*& elem2)
{
  PCB* temp = elem1; //Set a temp value to elem1

  //Swap elem1 with elem2
  elem1 = elem2;
  elem2 = temp;
}

/*
 * @brief Fixes any heap violations from a specified index going up the heap
*/
void ReadyQueue::bubbleUp(unsigned int idx) {
  unsigned int parent = (idx - 1)/2; //index to capture the parent of the heap

  //Loop while the child is greater than the parent and not at the root of the heap
  while((idx > 0 && idx < 200)&& maxHeap[idx]->getPriority() > maxHeap[parent]->getPriority())
    {
      exchange(maxHeap[idx], maxHeap[parent]); //Swap child and parent
      idx = parent; //Set new child
      parent = (parent - 1)/2; //Set new parent
    }
}

/*
 * @brief Fixes any heap violations from a specified index going down the heap 
*/
void ReadyQueue::maxHeapify(unsigned int idx) {
  unsigned int l = (2*idx) + 1; //the index of the left child of i
  unsigned int r = (2*idx) + 2; //the index of the right child of i
  unsigned int largest = idx; //the index of the largest value among the values at i, l and r

  if(l < count && maxHeap[l]->getPriority() > maxHeap[largest]->getPriority()) //Check if left heap is larger and in range
    largest = l; //Change largest to l
  if(r < count && maxHeap[r]->getPriority() > maxHeap[largest]->getPriority()) //Check if right heap is larger and in range
    largest = r; //Change largest to r

  //The largest is either the right or left child. We need to exchange the parent with it.
  if(largest != idx)
    {
      exchange(maxHeap[largest], maxHeap[idx]); //exchange the 2 values

      maxHeapify(largest); //There might be a violation at the position that was exchanged
    }
}
