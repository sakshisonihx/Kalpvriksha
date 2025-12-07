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
        if (currentProcess != NULL && currentProcess->processState == TERMINATED)
        {
            currentProcess = NULL;
        }
        if (!processIsRunning && readyQueue.front != NULL)
        {
            currentProcess = readyQueue.front;
            processIsRunning = 1;
            readyQueue.front = currentProcess->next;
            if (readyQueue.front == NULL)
            {
                readyQueue.rear = NULL;
            }
            else
            {
                readyQueue.front->previous = NULL;
            }
            currentProcess->processState = RUNNING;
            currentProcess->next = NULL;
            currentProcess->previous = NULL;
        }
        if (processIsRunning && currentProcess != NULL)
        {
            executeCurrentProcess(currentProcess);
            if (!processIsRunning)
            {
                currentProcess = NULL;
            }
        }
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
        KilledProcess *nextProcess = temp->next;
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
                    currentProcess->completionTime = systemClock + 1;

                    // removing process from queues
                    if (currentProcess->processState == RUNNING)
                    {
                        processIsRunning = 0;
                    }
                    else
                    {
                        if (readyQueue.front == currentProcess)
                        {
                            readyQueue.front = currentProcess->next;
                            if (readyQueue.front)
                            {
                                readyQueue.front->previous = NULL;
                            }
                            else
                            { // queue became empty as the process was last in queue
                                readyQueue.rear = NULL;
                            }
                        }
                        else if (readyQueue.rear == currentProcess)
                        {
                            readyQueue.rear = currentProcess->previous;
                            if (readyQueue.rear)
                            {
                                readyQueue.rear->next = NULL;
                            }
                            else
                            {
                                readyQueue.front = NULL;
                            }
                        }
                        else if (waitingQueue.front == currentProcess)
                        {
                            waitingQueue.front = currentProcess->next;
                            if (waitingQueue.front)
                            {
                                waitingQueue.front->previous = NULL;
                            }
                            else
                            {
                                waitingQueue.rear = NULL;
                            }
                        }
                        else if (waitingQueue.rear == currentProcess)
                        {
                            waitingQueue.rear = currentProcess->previous;
                            if (waitingQueue.rear)
                            {
                                waitingQueue.rear->next = NULL;
                            }
                            else
                            {
                                waitingQueue.front = NULL;
                            }
                        }
                        else
                        {
                            currentProcess->previous->next = currentProcess->next;
                            currentProcess->next->previous = currentProcess->previous;
                        }
                    }
                    currentProcess->next = currentProcess->previous = NULL;
                    currentProcess->processState = TERMINATED;
                    currentProcess->waitingTime = currentProcess->turnAroundTime = -1;
                    currentProcess->killTime = temp->killTime;

                    // entering process in terminated queue
                    enqueueInQueue(&terminatedQueue.front, &terminatedQueue.rear, currentProcess);
                    break;
                }
                currentNode = currentNode->next;
            }
            // removing process from kill list
            if (temp->previous)
            {
                temp->previous->next = temp->next;
            }
            else
            {
                killedProcessListHead = temp->next;
            }
            if (temp->next)
            {
                temp->next->previous = temp->previous;
            }

            free(temp);
        }
        temp = nextProcess;
    }
}

void executeCurrentProcess(ProcessDetails *currentProcess)
{
    currentProcess->runningTime++;
    if (currentProcess->runningTime == currentProcess->burstTime)
    {
        processIsRunning = 0;
        currentProcess->processState = TERMINATED;
        currentProcess->completionTime = systemClock + 1;
        currentProcess->turnAroundTime = currentProcess->completionTime -
                                         currentProcess->arrivalTime;
        enqueueInQueue(&terminatedQueue.front, &terminatedQueue.rear, currentProcess);
        return;
    }

    if (currentProcess->runningTime == currentProcess->ioStartTime)
    {
        processIsRunning = 0;
        // done so that when waiting time is incremented for processes in waiting queue, this process that just entered waiting queue does not have its waiting time incremented for same tick.
        currentProcess->waitingTime--;
        currentProcess->processState = WAITING;
        currentProcess->ioRemainingTime = currentProcess->ioDuration + 1;
        enqueueInQueue(&waitingQueue.front, &waitingQueue.rear, currentProcess);
    }
}

void increaseWaitingTime()
{
    if (readyQueue.front != NULL)
    {
        ProcessDetails *temp = readyQueue.front;
        while (temp != NULL)
        {
            temp->waitingTime++;
            temp = temp->next;
        }
    }
    if (waitingQueue.front != NULL)
    {
        ProcessDetails *temp = waitingQueue.front;
        while (temp != NULL)
        {
            temp->waitingTime++;
            temp->ioRemainingTime--;

            ProcessDetails *nextProcess = temp->next;
            if (temp->ioRemainingTime == 0)
            {
                temp->processState = READY;
                temp->ioRemainingTime = temp->ioDuration;
                if (temp->previous)
                {
                    temp->previous->next = temp->next;
                }
                else
                {
                    waitingQueue.front = temp->next;
                }

                if (temp->next)
                {
                    temp->next->previous = temp->previous;
                }
                else
                {
                    waitingQueue.rear = temp->previous;
                }
                temp->next = temp->previous = NULL;
                enqueueInQueue(&readyQueue.front, &readyQueue.rear, temp);
            }
            temp = nextProcess;
        }
    }
}

void printOutput()
{
    if (terminatedQueue.front == NULL)
        return;

    int count = 0;
    ProcessDetails *temp = terminatedQueue.front;
    int anyKilled = 0;
    while (temp != NULL)
    {
        count++;
        if (temp->killTime != -1)
            anyKilled = 1;
        temp = temp->next;
    }

    ProcessDetails **arr = malloc(count * sizeof(ProcessDetails *));
    temp = terminatedQueue.front;
    int i = 0;
    while (temp != NULL)
    {
        arr[i++] = temp;
        temp = temp->next;
    }

    for (int a = 0; a < count - 1; a++)
    {
        for (int b = a + 1; b < count; b++)
        {
            if (arr[a]->processID > arr[b]->processID)
            {
                ProcessDetails *swap = arr[a];
                arr[a] = arr[b];
                arr[b] = swap;
            }
        }
    }

    if (anyKilled)
        printf("\n| %-6s | %-15s | %-6s | %-6s | %-12s | %-15s | %-15s |",
               "PID", "Name", "CPU", "IO", "Status", "Turnaround", "Waiting");
    else
        printf("\n| %-6s | %-15s | %-6s | %-6s | %-15s | %-15s |",
               "PID", "Name", "CPU", "IO", "Turnaround", "Waiting");

    for (int x = 0; x < count; x++)
    {
        ProcessDetails *p = arr[x];

        if (anyKilled)
        {
            if (p->killTime != -1)
            {
                printf("\n| %-6d | %-15s | %-6d | %-6d | %-9s %-2d | %-15s | %-15s |",
                       p->processID, p->processName, p->burstTime, p->ioDuration,
                       "KILLED at", p->killTime, "-", "-");
            }
            else
            {
                printf("\n| %-6d | %-15s | %-6d | %-6d | %-12s | %-15d | %-15d |",
                       p->processID, p->processName, p->burstTime, p->ioDuration,
                       "OK", p->turnAroundTime, p->waitingTime);
            }
        }
        else
        {
            printf("\n| %-6d | %-15s | %-6d | %-6d | %-15d | %-15d |",
                   p->processID, p->processName, p->burstTime,
                   p->ioDuration, p->turnAroundTime, p->waitingTime);
        }
    }
    free(arr);
}