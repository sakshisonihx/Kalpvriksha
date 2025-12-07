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
    }
}