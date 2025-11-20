#ifndef SORTING_FUNCTIONS_H
#define SORTING_FUNCTIONS_H

#include "Team_structure.h"

void insertionSort(TeamNode[]);
void heapifyUp(int);
void heapifyDown(int);
PlayerNode *extractMax();
void insertInHeap(PlayerNode *);

#endif