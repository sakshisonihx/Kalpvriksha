#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#define HASH_MAP_SIZE 100

typedef enum
{
    READY,
    RUNNING,
    WAITING,
    TERMINATED
} State;

typedef struct ProcessDetails
{
    int processID;
    char *processName;
    int burstTime;
    int ioStartTime;
    int ioDuration;
    State processState;
    int arrivalTime;
    int completionTime;
    int waitingTime;
    int turnAroundTime;
    int runningTime;
    struct ProcessDetails *next;
} ProcessDetails;

typedef struct
{
    ProcessDetails *front;
    ProcessDetails *rear;
} ReadyQueue;

typedef struct
{
    ProcessDetails *front;
    ProcessDetails *rear;
} WaitingQueue;

typedef struct
{
    ProcessDetails *front;
    ProcessDetails *rear;
} TerminatedQueue;

typedef struct HashNode
{
    ProcessDetails *processData;
    struct HashNode *next;
} HashNode;

extern HashNode *PCBHash[HASH_MAP_SIZE];
extern ReadyQueue readyQueue;
extern WaitingQueue waitingQueue;
extern TerminatedQueue terminatedQueue;
extern int killProcessArray[HASH_MAP_SIZE][2];
extern int killProcessCount;

// implementation function declaration
int getHashKey(int);
void insertInPCB(int, char[], int, int, int);
void updateKillTime(int, int);

#endif