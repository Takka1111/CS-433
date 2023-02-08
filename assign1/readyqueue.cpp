#include <iostream>
#include "readyqueue.h"

using namespace std;

//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient comments to your code

/*
 * @brief Constructor for the ReadyQueue class.
*/
ReadyQueue::ReadyQueue()  {
  count = 0;
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
    
  maxHeap[count++] = pcbPtr;
    
  bubbleUp(count - 1);
}

/*
 * @brief Remove and return the PCB with the highest priority from the queue
 *
 * @return PCB*: the pointer to the PCB with the highest priority
*/
PCB* ReadyQueue::removePCB() {
  // When removing a PCB from the queue, you must change its state to RUNNING.
  PCB* pcbPtr = maxHeap[0];
  
  exchange(maxHeap[0], maxHeap[count - 1]);
  count--;
  
  maxHeapify(0);
  
  pcbPtr->setState(ProcState::RUNNING);
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

//***********************************************************************************ADD COMMENTS

void ReadyQueue::exchange(PCB*& elem1, PCB*& elem2)
{
  PCB* temp = elem1; //Set a temp value to elem1

  //Swap elem1 with elem2
  elem1 = elem2;
  elem2 = temp;
}

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

void ReadyQueue::maxHeapify(unsigned int idx) {
  //Declare variables
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
