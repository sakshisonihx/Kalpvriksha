#include "Sorting_functions.h"
#include <stdio.h>

void insertionSort(TeamNode array[]){
for (int i = 1; i < MAX_TEAMS; i++)
    {
        TeamNode currentTeam = array[i];
        int j = i - 1;
        while (j >= 0 && array[j].teamData.averageBattingStrikeRate < currentTeam.teamData.averageBattingStrikeRate)
        {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = currentTeam;
    }
}

void heapifyUp(int index)
{
    int parent = (index - 1) / 2;
    if (index != 0 && maxHeap[index]->playerData.performanceIndex > maxHeap[parent]->playerData.performanceIndex)
    {
        PlayerNode *temp = maxHeap[index];
        maxHeap[index] = maxHeap[parent];
        maxHeap[parent] = temp;
        heapifyUp(parent);
    }
}

void heapifyDown(int index)
{
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;
    int largest = index;
    if (leftChild < totalHeapElements && maxHeap[leftChild]->playerData.performanceIndex > maxHeap[largest]->playerData.performanceIndex)
    {
        largest = leftChild;
    }
    if (rightChild < totalHeapElements && maxHeap[rightChild]->playerData.performanceIndex > maxHeap[largest]->playerData.performanceIndex)
    {
        largest = rightChild;
    }
    if (largest != index)
    {
        PlayerNode *temp = maxHeap[largest];
        maxHeap[largest] = maxHeap[index];
        maxHeap[index] = temp;
        heapifyDown(largest);
    }
}

PlayerNode *extractMax()
{
    if (totalHeapElements <= 0)
    {
        return NULL;
    }
    PlayerNode *element = maxHeap[0];
    maxHeap[0] = maxHeap[totalHeapElements - 1];
    totalHeapElements--;
    heapifyDown(0);
    return element;
}

void insertInHeap(PlayerNode *element)
{
    if (totalHeapElements >= MAX_TEAMS)
    {
        return;
    }
    maxHeap[totalHeapElements] = element;
    heapifyUp(totalHeapElements);
    totalHeapElements++;
}