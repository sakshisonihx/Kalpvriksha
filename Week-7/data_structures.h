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

typedef struct
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
} ProcessDetails;

typedef struct QueueNode
{
    ProcessDetails processData;
    struct QueueNode *next;
    struct QueueNode *previous;
} QueueNode;

typedef struct
{
    QueueNode *front;
    QueueNode *rear;
} ReadyQueue;

typedef struct
{
    QueueNode *front;
    QueueNode *rear;
} WaitingQueue;

typedef struct
{
    QueueNode *front;
    QueueNode *rear;
} TerminatedQueue;

typedef struct PCBNode
{
    ProcessDetails processData;
    struct PCBNode *next;
} PCBNode;

extern PCBNode *PCBHash[HASH_MAP_SIZE];

// implementation function declaration
int getHashKey(int key);

#endif