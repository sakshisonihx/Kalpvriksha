#include "data_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HashNode *PCBHash[HASH_MAP_SIZE] = {0};
ReadyQueue readyQueue = {0};
WaitingQueue waitingQueue = {0};
TerminatedQueue terminatedQueue = {0};
KilledProcess *killedProcessListHead = NULL;

int getHashKey(int key)
{
    return key % HASH_MAP_SIZE;
}

void insertInPCB(int pid, char *pname, int burstTime, int ioTime, int ioDuration)
{
    ProcessDetails *newProcess = (ProcessDetails *)malloc(sizeof(ProcessDetails));
    if (!newProcess)
    {
        printf("\nMemory allocation failed");
    }
    newProcess->processID = pid;
    newProcess->processName = calloc(strlen(pname), sizeof(char));
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
    newProcess->ioRemainingTime = ioDuration;
    newProcess->next = NULL;
    newProcess->previous = NULL;

    if (readyQueue.rear == NULL)
    {
        readyQueue.rear = newProcess;
        readyQueue.front = newProcess;
    }
    else
    {
        readyQueue.rear->next = newProcess;
        newProcess->previous = readyQueue.rear;
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
        if (killedProcessListHead == NULL)
        {
            KilledProcess *newKilledProcess = (KilledProcess *)malloc(sizeof(KilledProcess));
            if (newKilledProcess == NULL)
            {
                printf("\nMemory allocation failed");
                return;
            }
            newKilledProcess->pid = pid;
            newKilledProcess->killTime = time;
            newKilledProcess->next = NULL;
            newKilledProcess->previous = NULL;
            return;
        }

        KilledProcess *temp = killedProcessListHead;
        while (temp != NULL)
        {
            if (temp->pid == pid)
            {
                temp->killTime = time;
                break;
            }
            temp = temp->next;
        }
        if (temp == NULL)
        {
            KilledProcess *newKilledProcess = (KilledProcess *)malloc(sizeof(KilledProcess));
            if (newKilledProcess == NULL)
            {
                printf("\nMemory allocation failed");
                return;
            }
            newKilledProcess->pid = pid;
            newKilledProcess->killTime = time;
            temp->previous->next = newKilledProcess;
            newKilledProcess->previous = temp;
            newKilledProcess->next = NULL;
        }
    }
    else
    {
        printf("\nError. Process with specified PID not found");
        return;
    }
}