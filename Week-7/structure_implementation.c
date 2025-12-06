#include "data_structures.h"
#include <stdio.h>
#include <stdlib.h>

HashNode *PCBHash[HASH_MAP_SIZE] = {0};
ReadyQueue readyQueue = {0};
WaitingQueue waitingQueue = {0};
TerminatedQueue terminatedQueue = {0};

int getHashKey(int key)
{
    return key % HASH_MAP_SIZE;
}

void insertInPCB(int pid, char pname[], int burstTime, int ioTime, int ioDuration)
{
    ProcessDetails *newProcess = (ProcessDetails *)malloc(sizeof(ProcessDetails));
    if (!newProcess)
    {
        printf("\nMemory allocation failed");
    }
    newProcess->processID = pid;
    newProcess->processName = calloc(sizeof(pname), sizeof(char));
    strcpy(newProcess->processName, pname);
    newProcess->burstTime = burstTime;
    newProcess->ioDuration = ioDuration;
    newProcess->ioStartTime = ioTime;
    newProcess->arrivalTime = 0;
    newProcess->completionTime = -1;
    newProcess->processState = READY;
    newProcess->turnAroundTime = -1;
    newProcess->waitingTime = -1;
    newProcess->killTime = -1;
    newProcess->next = NULL;

    if (readyQueue.rear == NULL)
    {
        readyQueue.rear = newProcess;
        readyQueue.front = newProcess;
    }
    else
    {
        readyQueue.rear->next = newProcess;
        readyQueue.rear = newProcess;
    }

    HashNode *newHashNode = (HashNode *)malloc(sizeof(HashNode));
    if (!newHashNode)
    {
        printf("\nMemory allocation failed");
    }
    newHashNode->next = NULL;
    newHashNode->processData = newProcess;

    int hashIndex = getHashKey(pid);
    newHashNode->next = PCBHash[hashIndex];
    PCBHash[hashIndex] = newHashNode;
}

void updateKillTime(int pid, int time)
{
    int hashIndex = getHashKey(pid);
    HashNode *currentHashNode = PCBHash[hashIndex];
    while (currentHashNode != NULL)
    {
        if (currentHashNode->processData->processID == pid)
        {
            currentHashNode->processData->killTime = time;
            return;
        }
        currentHashNode = currentHashNode->next;
    }
    printf("\nError. Process with specified PID not found");
}