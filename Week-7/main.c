#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_structures.h"

#define INPUT_SIZE 100

int systemClock = 0;
int processIsRunning = 0;

void readLine(char *);
void beginExecution();
void executeCurrentProcess(ProcessDetails *);
void increaseWaitingTime();
void checkKillProcess();
void printOutput();

int main()
{
    char inputBuffer[INPUT_SIZE];
    while (1)
    {
        if (!fgets(inputBuffer, INPUT_SIZE, stdin))
            break;
        inputBuffer[strcspn(inputBuffer, "\n")] = '\0';
        if (strcmp(inputBuffer, "END") == 0)
        {
            break;
        }
        readLine(inputBuffer);
    }
    beginExecution();
    printOutput();
    return 0;
}

void readLine(char *line)
{
    if (strlen(line) == 0)
    {
        return;
    }

    if (strncmp(line, "KILL", 4) == 0)
    {
        int pid, killTime;
        if (sscanf(line, "KILL %d %d", &pid, &killTime) == 2)
        {
            updateKillTime(pid, killTime);
        }
        else
        {
            printf("\nInvalid command");
        }
    }
    else
    {
        char name[50];
        int pid, burstTime, ioTime, ioDuration;
        if (sscanf(line, "%s %d %d %d %d", name, &pid, &burstTime, &ioTime, &ioDuration) == 5)
        {
            insertInPCB(pid, name, burstTime, ioTime, ioDuration);
        }
        else
        {
            printf("\nInvalid command");
        }
    }
}

void beginExecution()
{
    ProcessDetails *currentProcess = NULL;
    while (readyQueue.front != NULL || waitingQueue.front != NULL || processIsRunning)
    {
        checkKillProcess();
        if (!processIsRunning && readyQueue.front != NULL)
        {
            currentProcess = readyQueue.front;
            processIsRunning = 1;
            readyQueue.front = currentProcess->next;
            currentProcess->processState = RUNNING;
            currentProcess->next = NULL;
        }
        executeCurrentProcess(currentProcess);
        increaseWaitingTime();
        systemClock++;
    }
}

// checking whether any process got killed in current tick
void checkKillProcess()
{
    if (killedProcessListHead == NULL)
    {
        return;
    }

    KilledProcess *temp = killedProcessListHead;
    while (temp != NULL)
    {
        if (temp->killTime == systemClock)
        {
            int hashIndex = getHashKey(temp->pid);
            HashNode *currentNode = PCBHash[hashIndex];
            while (currentNode != NULL)
            {
                // process found
                if (currentNode->processData->processID == temp->pid)
                {
                    ProcessDetails *currentProcess = currentNode->processData;
                    currentProcess->completionTime = systemClock;

                    // removing process from queues
                    if (readyQueue.front == currentProcess)
                    {
                        readyQueue.front = currentProcess->next;
                    }
                    else if (readyQueue.rear == currentProcess)
                    {
                        readyQueue.rear = currentProcess->previous;
                    }
                    else if (waitingQueue.front == currentProcess)
                    {
                        waitingQueue.front = currentProcess->next;
                    }
                    else if (waitingQueue.rear == currentProcess)
                    {
                        waitingQueue.rear = currentProcess->previous;
                    }
                    else
                    {
                        currentProcess->previous->next = currentProcess->next;
                        currentProcess->next->previous = currentProcess->previous;
                    }
                    currentProcess->next = NULL;
                    currentProcess->previous = NULL;
                    if (currentProcess->processState == RUNNING)
                    {
                        processIsRunning = 0;
                    }

                    currentProcess->processState = TERMINATED;

                    // entering process in terminated queue
                    enqueueInQueue(terminatedQueue.front, terminatedQueue.rear, currentProcess);
                    break;
                }
                currentNode = currentNode->next;
            }
        }

        // removing process from kill list
        if (temp->previous)
        {
            temp->previous->next = temp->next;
        }
        if (temp->next)
        {
            temp->next->previous = temp->previous;
        }

        temp = temp->next;
        free(temp->previous);
    }
}

void executeCurrentProcess(ProcessDetails *currentProcess)
{
    if (currentProcess->runningTime == currentProcess->burstTime)
    {
        processIsRunning = 0;
        currentProcess->processState = TERMINATED;
        currentProcess->completionTime = systemClock;
        currentProcess->turnAroundTime = currentProcess->completionTime -
                                         currentProcess->arrivalTime;
        enqueueInQueue(terminatedQueue.front, terminatedQueue.rear, currentProcess);
        return;
    }

    if (currentProcess->runningTime == currentProcess->ioStartTime)
    {
        processIsRunning = 0;
        currentProcess->processState = WAITING;
        enqueueInQueue(waitingQueue.front, waitingQueue.rear, currentProcess);
    }
    currentProcess->runningTime++;
}

void increaseWaitingTime()
{
    if (waitingQueue.front != NULL)
    {
        ProcessDetails *temp = waitingQueue.front;
        while (temp != NULL)
        {
            temp->waitingTime++;
            temp->ioRemainingTime--;

            if (temp->ioRemainingTime == 0)
            {
                temp->processState = READY;
                if (temp->previous)
                {
                    temp->previous->next = temp->next;
                }
                if (temp->next)
                {
                    temp->next->previous = temp->previous;
                }
                temp->next = NULL;
                enqueueInQueue(readyQueue.front, readyQueue.rear, temp);
            }
            temp = temp->next;
        }
    }

    if (readyQueue.front != NULL)
    {
        ProcessDetails *temp = readyQueue.front;
        while (temp != NULL)
        {
            temp->waitingTime++;
            temp = temp->next;
        }
    }
}

void printOutput()
{
    if (terminatedQueue.front == NULL)
    {
        return;
    }
    printf("\n| %-6d | %-15s | %-6d | %-6d | %-6d | %-6d |", "PID", "Name", "CPU", "IO", "Turnaround", "Waiting");
    ProcessDetails *temp = terminatedQueue.front;
    while (temp != NULL)
    {
        printf("\n| %-6d | %-15s | %-6d | %-6d | %-6d | %-6d |", temp->processID, temp->processName,
               temp->burstTime, temp->ioDuration, temp->turnAroundTime, temp->waitingTime);
        temp = temp->next;
    }
}