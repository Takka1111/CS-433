/*
 * Assignment 1: priority queue of processes
 * @file pcbtable.h
 * @author Tucker Shaw and Abraham Gomez
 * @brief This is the implementation file for the PCBTable class.
*/

#include "pcbtable.h"

/*
 * @brief Construct a new PCBTable object of the given size (number of PCBs)
 *
 * @param size: the capacity of the PCBTable
*/
PCBTable::PCBTable(int size) {
  this->table.resize(size); //Set the size of the vector to input size
}

/*
 * @brief Destroy the PCBTable object. Make sure to delete all the PCBs in the table.
 *
*/
PCBTable::~PCBTable() {
  //Iterate through the table and destroy all PCB objects	
  for(vector<PCB*>::iterator i = table.begin(); i != table.end(); i++) {
    delete *i;
  } 
}

/*
 * @brief Get the PCB at index "idx" of the PCBTable.
 *
 * @param idx: the index of the PCB to get
 * @return PCB*: pointer to the PCB at index "idx"
*/
PCB* PCBTable::getPCB(unsigned int idx) {
  return table[idx];
}

/*
 * @brief Add a PCB to the PCBTable.
 *
 * @param pcb: the PCB to add
*/
void PCBTable::addPCB(PCB *pcb, unsigned int idx) {
  table[idx] = pcb;
}
