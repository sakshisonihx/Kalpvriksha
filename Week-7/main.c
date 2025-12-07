#include <stdio.h>
#include <string.h>
#include "data_structures.h"

#define INPUT_SIZE 100

int systemClock = 0;
int processIsRunning = 0;

void readLine(char *);
void executeProcesses();

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
    executeProcesses();
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
            // printf("\nKILL command, pid = %d, kill time = %d", pid, killTime);
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
            // printf("\nProcess detail, pid = %d, name = %s, burstTime = %d, iotime = %d, ioDuration = %d", pid, name, burstTime, ioTime, ioDuration);
        }
        else
        {
            printf("\nInvalid command");
        }
    }
}

void executeProcesses()
{
    while (readyQueue.front != NULL || waitingQueue.front != NULL || processIsRunning)
    {
        checkKillProcess();
    }
}

// checking whether any process got killed in current tick
void checkKillProcess()
{
    if (KilledProcessListHead == NULL)
    {
        return;
    }

    KilledProcess *temp = KilledProcessListHead;
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
                    currentProcess->processState = TERMINATED;

                    // entering process in terminated queue
                    if (terminatedQueue.rear == NULL)
                    {
                        terminatedQueue.rear = currentProcess;
                        terminatedQueue.front = currentProcess;
                    }
                    else
                    {
                        terminatedQueue.rear->next = currentProcess;
                        currentProcess->previous = terminatedQueue.rear;
                        terminatedQueue.rear = currentProcess;
                    }
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