#include "data_structures.h"
#include <stdio.h>
#include <stdlib.h>

HashNode *PCBHash[HASH_MAP_SIZE] = {0};
ReadyQueue readyQueue = {0};
WaitingQueue waitingQueue = {0};
TerminatedQueue terminatedQueue = {0};
int killProcessArray[HASH_MAP_SIZE][2] = {0};
int killProcessCount = 0;

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
    newProcess->runningTime = 0;
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
    int hashKey = getHashKey(pid);
    HashNode *currentHashNode = PCBHash[hashKey];
    int processExists = 0;
    while (currentHashNode != NULL)
    {
        if (currentHashNode->processData->processID == pid)
        {
            processExists = 1;
            break;
        }
        currentHashNode = currentHashNode->next;
    }
    if (processExists)
    {
        int found = 0;
        for (int i = 0; i < killProcessCount; i++)
        {
            if (killProcessArray[i][0] == pid)
            {
                killProcessArray[i][1] = time;
                found = 1;
                break;
            }
        }
        if (killProcessCount == 0 || !found)
        {
            killProcessArray[killProcessCount][0] = pid;
            killProcessArray[killProcessCount][1] = time;
            killProcessCount++;
        }
    }
    else
    {
        printf("\nError. Process with specified PID not found");
        return;
    }
}